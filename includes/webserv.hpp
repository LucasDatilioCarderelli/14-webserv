#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <sys/socket.h> // for socket(), bind(), listen(), accept()
#include <netinet/in.h> // for sockaddr_in
#include <unistd.h> // for close()
#include <string.h> // for memset()
#include <iostream> // for std::cout
#include <csignal> // or #include <signal.h>
#include <cstdlib> // or #include <stdlib.h>
#include <vector> // for std::vector
#include "Logger.hpp" // For print logs
// #include "Server.hpp"


// Global logger object
static Logger logger;

#endif