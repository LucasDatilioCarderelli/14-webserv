#ifndef SERVER_HPP
#define SERVER_HPP

#include "webserv.hpp"
#include "Config.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "utils.hpp"

#include <poll.h>
#include <fcntl.h> // for non-blocking sockets
#include <cerrno> // for errno
#include <sys/poll.h> // for multiplexing I/O

#define MAXEVENTS 64 // Maximum number of events to be returned by poll_wait


class Server {
private:
    std::vector<ServerConfig>   _servers;
    std::vector<int>            _sockets;


public:
    Server();
    Server(std::vector<ServerConfig> servers);
    ~Server();

    void run();
    void handleClientRequest(int client_fd);

    void create_socket();
    void accept_connections();
};

#endif
