#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <csignal> // or #include <signal.h>
#include <cstdlib> // or #include <stdlib.h>
#include "Logger.hpp"


// Global logger object
Logger logger;