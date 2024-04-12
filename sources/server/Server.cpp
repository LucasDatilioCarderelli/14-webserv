#include "Server.hpp"
#include "Request.hpp"
#include "utils.hpp"
#include <poll.h>
#include "Config.hpp"


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
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd < 0) {
            logger.log("socket failed", Logger::ERROR);
            exit(-1);
        }

        // Anexando o socket à porta fornecida pela configuração do servidor
        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            logger.log("setsockopt", Logger::ERROR);
            exit(-1);
        }
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(stringToNumber(_servers[i].config.listen));

        // Bind the socket
        if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            logger.log("bind failed", Logger::ERROR);
            exit(-1);
        }

        // Listen for incoming connections
        if (listen(socket_fd, SOMAXCONN) < 0) {
            logger.log("listen", Logger::ERROR);
            exit(-1);
        }

        // Adicionar o descritor de arquivo do servidor à lista de descritores de arquivo do servidor
        _sockets.push_back(socket_fd);
    
        logger.log("Server listening on port: " + _servers[i].config.listen, Logger::INFO);
    }
}


void Server::accept_connections() {
    int num_fds = _sockets.size();
    struct pollfd poll_fds[num_fds];

    while (true) {
        for (int i = 0; i < num_fds; i++) {
            poll_fds[i].fd = _sockets[i];
            poll_fds[i].events = POLLIN;
        }

        if (poll(poll_fds, num_fds, -1) < 0) {
            logger.log("poll failed", Logger::ERROR);
            exit(-1);
        }

        for (int i = 0; i < num_fds; i++) {
            int server_socket = poll_fds[i].fd;
            if (poll_fds[i].revents & POLLIN) {
                struct sockaddr_in address;
                socklen_t addrlen = sizeof(address);

                int connection_socket = accept(server_socket, (struct sockaddr *)&address, &addrlen);
                if (connection_socket < 0) {
                    logger.log("accept failed", Logger::ERROR);
                    exit(-1);
                }

                // // Tornar o socket não bloqueante
                // int flags = fcntl(connection_socket, F_GETFL, 0);
                // if (flags == -1) {
                //     logger.log("fcntl get", Logger::ERROR);
                //     exit(-1);
                // }
                // flags |= O_NONBLOCK;
                // if (fcntl(connection_socket, F_SETFL, flags) == -1) {
                //     logger.log("fcntl set", Logger::ERROR);
                //     exit(-1);
                // }

                handleClientRequest(connection_socket);
            }
        }
    }
}


void Server::handleClientRequest(int connection_socket) {
    char buffer[1024] = {0};
    long valread = recv(connection_socket, buffer, sizeof(buffer), 0);
    if (valread < 0) {
        logger.log("recv", Logger::ERROR);
        return;
    }

    // Tratar a requisição
    Request request;
    request.parseRequest(buffer);

    Response responseClient(request, _servers);


    std::string header = "\r\nContent-Type: text/html\r\nConnection: Close\r\n\r\n";

    std::string statusCode = "";
    if (request.getPath() != "/") {
        statusCode = "404 Not Found";
    } else {
        statusCode = "200 OK";
    }
    std::string body = "";
    if (statusCode == "404 Not Found") {
        body = "<html><body><h1>404 Not Found</h1></body></html>";
    } else {
        body = "<html><body><h1>200 OK</h1></body></html>";
    }
    std::string response = "HTTP/1.1 " + statusCode + header + body + "\n";
    std::string logMessage = request.makelog() + " " + statusCode;
    send(connection_socket, response.c_str(), response.size(), 0);
    logger.log(logMessage, Logger::INFO);


    shutdown(connection_socket, SHUT_WR); // Encerra a escrita no socket
    close(connection_socket); // Fecha o socket
}
