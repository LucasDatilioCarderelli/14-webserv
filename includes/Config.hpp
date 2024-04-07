#include <fstream>
#include <iostream>
#include <vector>


struct DefaultConfig {
    std::string listen;
    std::string server_name;
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
    std::vector<LocationConfig> location;
};


class Config {
private:
    std::ifstream _configFile;

public:
    Config(const std::string& configFilePath);
    ~Config();
    
    std::vector<ServerConfig> servers;
    void parseConfigFile();
    void printServers();
};



/*
server {
    listen 8000                                 1 PORT
    server_name localhost                       2 HOST
    index index.html                            3 DEFAULT SERVER
    error_page 404 ./conf-4x04.html             4 ERROR PAGE
    client_max_body_size 100                    5 CLIENT BODY SIZE
    allowed_method GET POST DELETE              6.1 ACCEPT HTTP METHODS                  
    cgi .php ./www/cgi-bin/php-cgi              

    location /redirect_web {                    
        allow_methods GET
        http_redirection http://www.google.com  6.2 HTTP REDIRECTION
    }

    location /directory {
        autoindex on                            6.4 DIRECTORY LISTING
        allow_methods GET
    }

    location /teste2 {
        allow_methods POST GET
        root ./root                             
        autoindex off
        index teste.html                        6.5 DEFAULT FILE
        cgi .php ./www/cgi-bin/php-cgi          6.6 CGI SCRIPT
    }

    location /uploads {
        allow_methods POST GET DELETE           6.7 WORK WITH POST AND GET
        root ./root
        autoindex off
        index teste.html
        cgi .php ./www/cgi-bin/php-cgi          6.8.2 FULL PATH
    }
    
    root ./www                                  6.3 ROOT DIRECTORY
}
*/