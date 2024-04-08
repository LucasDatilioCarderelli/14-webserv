#include "Server.hpp"
#include "Request.hpp"


Server::Server() {
    logger.log("Webserv starting", Logger::INFO);
}

Server::~Server() {
    logger.log("Webserv stopping", Logger::INFO);
}

void Server::run() {
    int server_fd, new_socket, opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Criando o socket do servidor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        logger.log("socket failed", Logger::ERROR);
        return;
    }

    // Anexando o socket à porta 8082
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        logger.log("setsockopt", Logger::ERROR);
        return;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8082);
    logger.log("Server listening on port 8082", Logger::INFO);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        logger.log("bind failed", Logger::ERROR);
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        logger.log("listen", Logger::ERROR);
        return;
    }

    // Tornar o socket não bloqueante
    int flags = fcntl(server_fd, F_GETFL, 0);
    if (flags == -1) {
        logger.log("fcntl get", Logger::ERROR);
        return;
    }
    flags |= O_NONBLOCK;
    if (fcntl(server_fd, F_SETFL, flags) == -1) {
        logger.log("fcntl set", Logger::ERROR);
        return;
    }

    // Configurar epoll
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        logger.log("epoll_create1", Logger::ERROR);
        return;
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        logger.log("epoll_ctl", Logger::ERROR);
        return;
    }

    struct epoll_event *events = (struct epoll_event *)calloc(MAXEVENTS, sizeof event);

    // Loop principal
    while (1) {
        int n = epoll_wait(epoll_fd, events, MAXEVENTS, -1);
        for (int i = 0; i < n; i++) {
            if ((events[i].events & EPOLLERR) ||
                (events[i].events & EPOLLHUP) ||
                (!(events[i].events & EPOLLIN))) {
                logger.log("epoll error", Logger::ERROR);
                close(events[i].data.fd);
                continue;
            } else if (server_fd == events[i].data.fd) {
                while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) != -1) {
                    // Tornar o socket do cliente não bloqueante
                    flags = fcntl(new_socket, F_GETFL, 0);
                    if (flags == -1) {
                        logger.log("fcntl get", Logger::ERROR);
                        return;
                    }
                    flags |= O_NONBLOCK;
                    if (fcntl(new_socket, F_SETFL, flags) == -1) {
                        logger.log("fcntl set", Logger::ERROR);
                        return;
                    }

                    event.data.fd = new_socket;
                    event.events = EPOLLIN | EPOLLET;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1) {
                        logger.log("epoll_ctl", Logger::ERROR);
                        return;
                    }
                }
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    logger.log("accept", Logger::ERROR);
                    return;
                }
            } else {
                char buffer[1024] = {0};
                long valread = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (valread < 0) {
                    logger.log("recv", Logger::ERROR);
                    return;
                }

                // Tratar a requisição
                Request request;
                request.parseRequest(buffer);


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
                send(events[i].data.fd, response.c_str(), response.size(), 0);
                logger.log(logMessage, Logger::INFO);


                shutdown(events[i].data.fd, SHUT_WR); // Encerra a escrita no socket
                close(events[i].data.fd); // Fecha o socket
            }
        }
    }

    free(events);
}
