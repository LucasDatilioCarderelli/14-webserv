#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include "Config.hpp"

#include <fcntl.h> // for non-blocking sockets
#include <cerrno> // for errno
#include <sys/epoll.h> // for multiplexing I/O

#define MAXEVENTS 64 // Maximum number of events to be returned by epoll_wait


class Server {

public:
    Server();
    Server(std::vector<ServerConfig> servers);
    ~Server();

    void run();
};

#endif
