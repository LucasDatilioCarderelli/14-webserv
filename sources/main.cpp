#include "webserv.hpp"
#include "Server.hpp"
#include "Config.hpp"

static void sigint_handler(int sig) {
    (void)sig;
    std::cout << std::endl;
    logger.log("SIGINT received ", Logger::WARNING);
    running = false;
}

static bool isConfigFile(const char *path) {
    // check if the file path ends with .conf
    std::string str(path);
    if (str.find(".conf") == std::string::npos) {
        return false;
    }
    std::ifstream file(path);
    return file.good();
}

int main (int argc, char **argv) {
    std::string cofigFilePath;
    if (argc == 1) {
        cofigFilePath = "./config/default.conf";      
    } else if (argc > 2) {
        logger.log("Usage: ./webserv [config_file_path]", Logger::ERROR);
        return 1;
    } else if (!isConfigFile(argv[1])) {
        logger.log("Invalid config file path: " + std::string(argv[1]), Logger::ERROR);
        return 1;
    } else {
        cofigFilePath = argv[1];
    }
    logger.log("Config file path: " + cofigFilePath, Logger::INFO);

    // Register signal handler for SIGINT ^C (Ctrl+C)
    signal(SIGINT, sigint_handler);

    // Parse config file
    Config config(cofigFilePath);

    // Start the server
    Server server(config.servers);
    server.run();

    logger.log("Webserv stopping", Logger::INFO);
    return 0;
}