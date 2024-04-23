#include "Server.hpp"


Server::Server() {
    logger.log("Webserv starting", Logger::INFO);
}

Server::~Server() {
    logger.log("Webserv stopping", Logger::INFO);
}

Server::Server(std::vector<ServerConfig> servers) : _servers(servers) {
    logger.log("Webserv starting", Logger::INFO);
}

void Server::run() {
    create_socket();
    accept_connections();
}

void Server::create_socket() {
    for (int i = 0; i < (int)_servers.size(); i++) {
        // create a socket
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            logger.log("create socket failed", Logger::ERROR);
            close(socket_fd);
            exit(-1);
        }
        // attach the socket to the port provided
        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            logger.log("setsockopt failed", Logger::ERROR);
            close(socket_fd);
            exit(-1);
        }
        // set the socket to non-blocking
        if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0) {
            logger.log("fcntl failed", Logger::ERROR);
            close(socket_fd);
            exit(-1);
        }

        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(stringToNumber(_servers[i].config.listen));

        // Bind the socket to the port
        if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            logger.log("bind failed on: " + _servers[i].config.listen, Logger::ERROR);
            close(socket_fd);
        } else {
            // Listen for incoming connections on the socket
            if (listen(socket_fd, SOMAXCONN) < 0) {
                logger.log("listen failed", Logger::ERROR);
                close(socket_fd);
                exit(-1);
            }
            // add the server file descriptor to socket vector
            _sockets.push_back(socket_fd);
        
            logger.log("Server listening on: http://" + 
                _servers[i].config.server_name + ":" + 
                _servers[i].config.listen, Logger::INFO);
        }
    }
}


void Server::accept_connections() {
    int num_fds = _sockets.size();
    struct pollfd poll_fds[num_fds];
    int timeout = (5 * 60 * 1000); // 5 minute (in milliseconds)

    // loop for accepting connections
    while (true) {
        for (int i = 0; i < num_fds; i++) {
            poll_fds[i].fd = _sockets[i];
            poll_fds[i].events = POLLIN;
        }

        // wait for incoming connections
        if (poll(poll_fds, num_fds, timeout) <= 0) {
            logger.log("poll failed or timeout", Logger::WARNING);
            exit(-1);
        }

        // check for incoming connections
        for (int i = 0; i < num_fds; i++) {
            int sockfd = poll_fds[i].fd;
            if (poll_fds[i].revents & POLLIN) {
                struct sockaddr_in address;
                socklen_t addrlen = sizeof(address);

                int connection_socket = accept(sockfd, (struct sockaddr *)&address, &addrlen);
                if (connection_socket <= 0) {
                    logger.log("accept failed", Logger::ERROR);
                    close(sockfd);
                } else {
                    handleClientRequest(connection_socket);
                }   
            }
        }
    }
}


void Server::handleClientRequest(int connection_socket) {
    std::string buffer;
    const size_t bufferSize = 4096;
    char recv_buffer[bufferSize];
    int bytes_received = 0;

    // receive the request
    while ((bytes_received = recv(connection_socket, recv_buffer, sizeof(recv_buffer), 0)) > 0) {
        buffer.append(recv_buffer, bytes_received);
        if (buffer.find("\r\n\r\n") != std::string::npos) {
            break;
        }
    }

    // parse the request
    Request request;
    request.parseRequest(buffer);

    // make the response
    Response responseClient(request, _servers);
    std::string response = responseClient.makeResponse(request);

    // send the response
    send(connection_socket, response.c_str(), response.size(), 0);

    // close the connection
    shutdown(connection_socket, SHUT_WR);
    close(connection_socket);
}
