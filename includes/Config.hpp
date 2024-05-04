#include <fstream>
#include <iostream>
#include <vector>

#pragma once


struct DefaultConfig {
    std::string root;
    std::string index;
    std::string errorPage;
    std::string autoindex;
    std::string maxBodySize;
    std::string cgi;
    std::string allowedMethods;
    std::string httpRedirection;
};

struct LocationConfig {
    std::string location;
    DefaultConfig config;
};

struct ServerConfig {
    DefaultConfig config;
    std::string listen;
    std::string server_name;
    std::vector<LocationConfig> location;
};


class Config {
private:
    std::ifstream _configFile;
    void setConfigValue(ServerConfig& server, std::string& line);
    void setConfigValue(DefaultConfig& config, std::string& line);
    void heritageServer();

    LocationConfig parseLocation(std::string line);
    ServerConfig parseServer(std::string line);

public:
    Config(const std::string& configFilePath);
    ~Config();
    
    std::vector<ServerConfig> servers;
    void parseConfigFile();
};

void printServers(std::vector<ServerConfig>& servers, bool shouldPrintLocation);
void printServer(ServerConfig& server, bool shouldPrintLocation);
void printLocation(LocationConfig& location);
void printConfig(DefaultConfig& config);
