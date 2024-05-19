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
    std::vector<char*>  _envArray;
    Request             _request;
    std::string         _cgi_path;
    int                 _cgi_status;
    std::string         _cgi_response;

public:
    Cgi();
    ~Cgi();
    Cgi(std::string cgi_path, Request& request);

    std::map<std::string, std::string> createEnv();
    std::vector<char*> createEnvArray();
    void executeCgi();

    int getCgiStatus() const { return _cgi_status; }
    std::string getCgiResponse() const { return _cgi_response; }
};

#endif
