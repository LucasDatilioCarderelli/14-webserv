#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "MimeType.hpp"
#include "StatusCode.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "utils.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <sys/types.h>


static MimeType mimeType;
static StatusCode statusCode;

class Response {
private:
    std::vector<ServerConfig>   _servers;
    int                         _status;
    DefaultConfig*              _config;
    std::string                 _response;

public:
    Response();
    ~Response();
    // receive the client request and the server config
    Response(Request& request, std::vector<ServerConfig>& servers);
    
    void validateServer(Request& request, std::vector<ServerConfig>& servers);
    int validatePath(Request& request, ServerConfig& servers);
    void validateLocation(Request& request, LocationConfig& location);

    std::string makeResponse(Request& request);
};

#endif
