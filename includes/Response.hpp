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
#include <sys/stat.h>

#define DOUBLE_CRLF "\r\n\r\n"

static MimeType mimeType;
static StatusCode statusCode;

class Response {
private:
    DefaultConfig*              _config;
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
    DefaultConfig* getConfig(Request& request, ServerConfig& server);

    // directives
    std::string generateDirectoryListing(const std::string& path);
    std::string makeRedirection(const std::string& path);

    // GET
    std::string openFile(const std::string& path);

    // POST
    void saveFileFromRequestBody(const std::string& requestBody);

    // DELETE
    void deleteFile(const std::string& path);

    // Utils
    std::string convertPercentTwenty(const std::string& input);
    bool isDirectory(const std::string& path);
    bool isFile(const std::string& path);
    std::string getPath(Request& request);

    // setter
    void setStatus(int status) { _status = status; }
};

#endif
