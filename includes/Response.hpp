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

#define DOUBLE_CRLF "\r\n\r\n"

static MimeType mimeType;
static StatusCode statusCode;

class Response {
private:
    // Request*                    _request;
    DefaultConfig*              _config;
    // ServerConfig*               _server;
    // LocationConfig*             _location;
    std::string                 _response;
    int                         _status;
    std::string                 _responseContentType;

public:
    Response();
    ~Response();
    Response(Request& request, std::vector<ServerConfig>& servers);
    
    std::string makeResponse(Request& request);

    // validate
    void validateServer(Request& request, std::vector<ServerConfig>& servers);
    int validatePath(Request& request, ServerConfig& servers);
    void validateLocation(Request& request, LocationConfig& location);
    DefaultConfig* getConfig(Request& request, ServerConfig& server);

    // get
    std::string generateDirectoryListing(const std::string& path);
    std::string makeRedirection(const std::string& path);
    std::string htmlEncode(const std::string& data);
    std::string readUploadedFile(const std::string& path);

    // std::string getFileOrDirectory();
    std::string openFile(const std::string& path);
    std::string getPath(Request& request);

    // post
    void saveFileFromRequestBody(const std::string& requestBody);

    // delete

    // setter
    void setStatus(int status) { _status = status; }
};

#endif
