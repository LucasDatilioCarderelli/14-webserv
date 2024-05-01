#include "Config.hpp"
#include "utils.hpp"


Config::Config(const std::string& configFilePath) {
    _configFile.open(configFilePath.c_str());
    if (!_configFile.is_open()) {
        logger.log("Error: could not open config file", Logger::ERROR);
        exit(-1);
    }

    parseConfigFile();

    if (servers.empty()) {
        logger.log("Error: no server block found in config file", Logger::ERROR);
        exit(-1);
    }
}

Config::~Config() {
    _configFile.close();
}

void Config::setConfigValue(DefaultConfig& config, std::string& line) {
    line = trimLine(line);
    std::string key = line.substr(0, line.find(" "));
    std::string value = line.substr(line.find(" ") + 1);
    if (key == "root") {
        config.root = value;
    } else if (key == "index") {
        config.index = value;
    } else if (key == "error_page") {
        config.errorPage = value;
    } else if (key == "autoindex") {
        config.autoindex = value;
    } else if (key == "client_max_body_size") {
        config.maxBodySize = value;
    } else if (key == "cgi") {
        config.cgi = value;
    } else if (key == "allow_method") {
        config.allowedMethods = value;
    } else if (key == "http_redirection") {
        config.httpRedirection = value;
    }
}

void Config::setConfigValue(ServerConfig& server, std::string& line) {
    line = trimLine(line);
    std::string key = line.substr(0, line.find(" "));
    std::string value = line.substr(line.find(" ") + 1);
    if (key == "listen") {
        server.listen = value;
    } else if (key == "server_name") {
        server.server_name = value;
    } else {
        setConfigValue(server.config, line);
    }
}

void Config::parseConfigFile() {
    std::string line;
    while (std::getline(_configFile, line)) {
        if (line.find("server ") != std::string::npos) {
            ServerConfig server;
            server.listen = "8080";
            server.server_name = "localhost";
            while (std::getline(_configFile, line) && line.find("}") == std::string::npos) {
                line = trimLine(line);
                if (line.find("location ") != std::string::npos) {
                    LocationConfig location;
                    location.location = line.substr(line.find(" ") + 1, line.find("{") - line.find(" ") - 2);
                    while (std::getline(_configFile, line) && line.find("}") == std::string::npos) {
                        line = trimLine(line);
                        setConfigValue(location.config, line);
                    }
                    server.location.push_back(location);
                } else {
                    setConfigValue(server, line);
                }
            }
            servers.push_back(server);
        }
    }
    printServers(servers, true);
}

void printConfig(DefaultConfig& config) {
    std::cout << "ROOT          : " << config.root << std::endl;
    std::cout << "INDEX         : " << config.index << std::endl;
    std::cout << "ERROR_PAGE    : " << config.errorPage << std::endl;
    std::cout << "AUTOINDEX     : " << config.autoindex << std::endl;
    std::cout << "MAX_BODY_SIZE : " << config.maxBodySize << std::endl;
    std::cout << "CGI           : " << config.cgi << std::endl;
    std::cout << "METHODS       : " << config.allowedMethods << std::endl;
    std::cout << "REDIRECTION   : " << config.httpRedirection << std::endl;
    std::cout << std::endl;
}

void printLocation(LocationConfig& location) {
    std::cout << "Location " << location.location << std::endl;
    printConfig(location.config);
}

void printServer(ServerConfig& server, bool shouldPrintLocation) {
    std::cout << "Server " << server.server_name << ":" << server.listen << std::endl;
    printConfig(server.config);
    if (shouldPrintLocation) {
        for (size_t i = 0; i < server.location.size(); i++) {
            printLocation(server.location[i]);
        }
    }
}

void printServers(std::vector<ServerConfig>& servers, bool shouldPrintLocation) {
    for (size_t i = 0; i < servers.size(); i++) {
        printServer(servers[i], shouldPrintLocation);
    }
}
