#include "Webserv.hpp"


class Server {
public:
    Server() {
        logger.log("Webserv starting", Logger::INFO);
    }

    void run() {    
        int server_fd, new_socket, opt = 1;
        struct sockaddr_in address;
        int addrlen = sizeof(address);
        char buffer[1024];

        memset(buffer, 0, sizeof(buffer)); // Inicializando o buffer

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
        
        // Bind the socket
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            logger.log("bind failed", Logger::ERROR);
            return;
        } else {
            logger.log("bind success", Logger::INFO);
        }

        // Listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            logger.log("listen", Logger::ERROR);
            return;
        } else {
            logger.log("Webserver running", Logger::INFO);
        }

        while (1) { // Loop infinito
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                logger.log("accept", Logger::ERROR);
                return;
            }

            std::string header = "\r\nContent-Type: text/plain\r\nConnection: Close\r\n\r\n";
            std::string response = "HTTP/1.1 200 OK" + header + "Hello, world!\n";
            send(new_socket, response.c_str(), response.size(), 0);
            logger.log("\"GET /hello HTTP1.1\" 200", Logger::INFO);
            shutdown(new_socket, SHUT_WR); // Encerra a escrita no socket
            // Espera o cliente fechar a conexão
            char buffer[1];
            while (recv(new_socket, buffer, sizeof(buffer), 0) > 0)
            close(new_socket); // Fecha o socket
        }
    }
};


void sigint_handler( int sig ) {
    // convert sig in string
    logger.log("SIGINT received ", Logger::INFO);
    exit(sig);
}


int main (int argc, char **argv) {
    (void)argc;
    (void)argv;

    // Associate the signal handler sigint_handler with SIGINT
    signal(SIGINT, sigint_handler);

    Server server;
    server.run();

    return 0;
}