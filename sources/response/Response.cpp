#include "Response.hpp"
#include "Cgi.hpp"


Response::Response() {}
Response::~Response() {}
Response::Response(Request& request, std::vector<ServerConfig>& servers) {
    _status = 200;
    validateServer(request, servers);
}


DefaultConfig* Response::getConfig(Request& request, ServerConfig& server) {
    for (size_t i = 0; i < server.location.size(); i++) {
        if (request.getPath() == server.location[i].location) {
            return &server.location[i].config;
        }
    }
    return &server.config;
}

void Response::validateServer(Request& request, std::vector<ServerConfig>& servers) {
    // get the server config
    ServerConfig* server = NULL;
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].listen == request.getPort()) {
            server = &servers[i];
        }
    }
    _config = getConfig(request, *server);

    // validate the request
    if (server->server_name != request.getHost()) {
        setStatus(502); // Bad Gateway
    }
    else if (request.getVersion() != "HTTP/1.1") {
        setStatus(505); // HTTP Version Not Supported
    }
    else if (request.getMethod() != "GET" && request.getMethod() != "POST" && 
             request.getMethod() != "DELETE") {
        setStatus(501); // Not Implemented
    }
    else if (_config->allowedMethods.find(request.getMethod()) == std::string::npos &&
             _config->allowedMethods != "") {
        setStatus(405); // Method Not Allowed
    }
    else if (stringToNumber(request.getContentLength()) > stringToNumber(_config->maxBodySize)) {
        setStatus(413); // Payload Too Large
    }
}

std::string Response::makeResponse(Request& request) {
    std::string body;
    std::string path = getPath(request);
    std::string error_page;

    if (_status != 200) {
        error_page = getErrorPage();
        body = openFile(error_page);
    }
    else if (_config->autoindex == "on" && request.getMethod() == "GET") {
        body = generateDirectoryListing(path);
    } 
    else if (_config->httpRedirection != "") {
        body = makeRedirection(_config->httpRedirection);
    } 
    else if (_config->cgi != "") {
        Cgi cgi(_config->cgi, request);
        body = cgi.executeCgi();
        if (body.empty()) {
            setStatus(500);
            error_page = getErrorPage();
            body = openFile(error_page);
        }
    } 
    else if (request.getMethod() == "GET") {
        path = (path[path.size() - 1] == '/') ? path + _config->index : path;
        body = openFile(path);
    }
    else if (request.getMethod() == "POST") {
        if (request.getContentType() == "multipart/form-data") {
            try {
                saveFileFromRequestBody(request.getBody(), path);
            } catch (const std::exception& e) {
                setStatus(400);
            }
        } else {
            setStatus(415);
        }
    }
    else if (request.getMethod() == "DELETE") {
        deleteFile(path);
    }
    else {
        setStatus(500);
        body = openFile("./www/statusCode/" + numberToString(_status) + ".html");
    }

    std::string contentType = "\r\nContent-Type: " + _responseContentType;
    std::string header = contentType + "\r\nConnection: Close\r\nServer: WebServ\r\n\r\n";

    std::string response = "HTTP/1.1 " + numberToString(_status) + " " + statusCode.get(_status) + header + body + "\n";
    std::string logMessage = request.makelog() + " " + numberToString(_status) + " " + statusCode.get(_status);
    if (_status >= 400) {
        logger.log(logMessage, Logger::ERROR);
    } else {
        logger.log(logMessage, Logger::INFO);
    }
    return response; 
}
