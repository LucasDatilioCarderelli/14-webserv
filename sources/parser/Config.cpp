#include "Config.hpp"
#include "utils.hpp"


static std::string trimLine(const std::string& line);


Config::Config(const std::string& configFilePath) {
    _configFile.open(configFilePath.c_str());
    if (!_configFile.is_open()) {
        throw std::runtime_error("Error: could not open config file");
    }
    parseConfigFile();
}

Config::~Config() {
    _configFile.close();
}

void setConfigValue(DefaultConfig& config, std::string& line) {
    line = trimLine(line);
    std::string key = line.substr(0, line.find(" "));
    std::string value = line.substr(line.find(" ") + 1);
    if (key == "listen") {
        config.listen = value;
    } else if (key == "server_name") {
        config.server_name = value;
    } else if (key == "root") {
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

void Config::parseConfigFile() {
    std::string line;
    while (std::getline(_configFile, line)) {
        if (line.find("server ") != std::string::npos) {
            ServerConfig server;
            while (std::getline(_configFile, line) && line.find("}") == std::string::npos) {
                line = trimLine(line);
                if (line.find("location ") != std::string::npos) {
                    LocationConfig location;
                    location.location = line.substr(line.find(" ") + 1);
                    while (std::getline(_configFile, line) && line.find("}") == std::string::npos) {
                        line = trimLine(line);
                        setConfigValue(location.config, line);
                    }
                    server.location.push_back(location);
                } else {
                    setConfigValue(server.config, line);
                }
            }
            servers.push_back(server);
        }
    }
    // printServers(servers);
}

void printLocation(LocationConfig& location) {
    std::cout << "Location " << location.location << std::endl;
    std::cout << "SERVER_NAME   : " << location.config.server_name << std::endl;
    std::cout << "ROOT          : " << location.config.root << std::endl;
    std::cout << "INDEX         : " << location.config.index << std::endl;
    std::cout << "ERROR_PAGE    : " << location.config.errorPage << std::endl;
    std::cout << "AUTOINDEX     : " << location.config.autoindex << std::endl;
    std::cout << "MAX_BODY_SIZE : " << location.config.maxBodySize << std::endl;
    std::cout << "CGI           : " << location.config.cgi << std::endl;
    std::cout << "METHODS       : " << location.config.allowedMethods << std::endl;
    std::cout << "REDIRECTION   : " << location.config.httpRedirection << std::endl;
    std::cout << std::endl;
}

void printServers(std::vector<ServerConfig>& servers) {
    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << "Server " << i << std::endl;
        std::cout << "PORT          : " << servers[i].config.listen << std::endl;
        std::cout << "SERVER_NAME   : " << servers[i].config.server_name << std::endl;
        std::cout << "ROOT          : " << servers[i].config.root << std::endl;
        std::cout << "INDEX         : " << servers[i].config.index << std::endl;
        std::cout << "ERROR_PAGE    : " << servers[i].config.errorPage << std::endl;
        std::cout << "AUTOINDEX     : " << servers[i].config.autoindex << std::endl;
        std::cout << "MAX_BODY_SIZE : " << servers[i].config.maxBodySize << std::endl;
        std::cout << "CGI           : " << servers[i].config.cgi << std::endl;
        std::cout << "METHODS       : " << servers[i].config.allowedMethods << std::endl;
        std::cout << "REDIRECTION   : " << servers[i].config.httpRedirection << std::endl;
        std::cout << std::endl;
        for (size_t j = 0; j < servers[i].location.size(); j++) {
            printLocation(servers[i].location[j]);
        }
    }
}

static std::string trimLine(const std::string& line) {
    std::string trimmedLine = line;
    trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));
    trimmedLine.erase(trimmedLine.find_last_not_of(" \t") + 1);
    return trimmedLine;
}
