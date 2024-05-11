#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"
#include "Request.hpp"

#include <map>
#include <vector>
#include <cstring>
#include <sys/wait.h>

class Cgi {
private:
    std::string         _cgi_path;
    Request             _request;
    std::vector<char*>  _envArray;

public:
    Cgi();
    ~Cgi();
    Cgi(std::string cgi_path, Request& request);

    std::map<std::string, std::string> createEnv();
    std::vector<char*> createEnvArray();
    std::string executeCgi();
};

#endif
