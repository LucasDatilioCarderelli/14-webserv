#include "webserv.hpp"
#include "Server.hpp"
#include "Config.hpp"


void sigint_handler(int sig) {
    logger.log("SIGINT received ", Logger::WARNING);
    exit(sig);
}


int main (int argc, char **argv) {
    std::string cofigFilePath;
    if (argc == 1) {
        cofigFilePath = "./config/default.conf";      
    } else {
        cofigFilePath = argv[1];
    }
    logger.log("Config file path: " + cofigFilePath, Logger::INFO);

    // Register signal handler for SIGINT ^C (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Parse config file
    Config config(cofigFilePath);

    // Iniciar o servidor
    Server server;
    server.run();

    return 0;
}