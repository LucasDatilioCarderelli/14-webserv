#include "Webserv.hpp"
#include "Server.hpp"


void sigint_handler(int sig) {
    logger.log("SIGINT received ", Logger::WARNING);
    exit(sig);
}


int main (int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Register signal handler for SIGINT ^C (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Iniciar o servidor
    Server server;
    server.run();

    return 0;
}