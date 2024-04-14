#include "Response.hpp"


Response::Response() {}
Response::~Response() {}
Response::Response(Request& request, std::vector<ServerConfig>& servers) : _servers(servers) {
    std::cout << "Response constructor" << std::endl;

    std::cout << "Request method: " << request.getMethod() << std::endl;
    std::cout << "Request path: " << request.getPath() << std::endl;
    std::cout << "Request version: " << request.getVersion() << std::endl;
    std::cout << "Request host: " << request.getHost() << std::endl;
    std::cout << "Request port: " << request.getPort() << std::endl;
    std::cout << "Request content type: " << request.getContentType() << std::endl;
    std::cout << "Request content length: " << request.getContentLength() << std::endl;
    std::cout << "Request body: " << request.getBody() << std::endl;

    validateServer(request, servers);
}


void Response::validateServer(Request& request, std::vector<ServerConfig>& servers) {
    _status = 200;
    // validate the port and host
    for (size_t i = 0; i < servers.size(); i++) {

        if (servers[i].config.listen == request.getPort() && servers[i].config.server_name == request.getHost()) {
            validatePath(request, servers[i]);
            break;
        }
        else {
            _status = 502;
        }

    }
 
    std::cout << "\n" << _status << " " << statusCode.get(_status) << std::endl;

}


int Response::validatePath(Request& request, ServerConfig& server) {
    // validate the path
    if (request.getPath() == "/") {
        std::cout << "PORT          : " << server.config.listen << std::endl;
        std::cout << "SERVER_NAME   : " << server.config.server_name << std::endl;
        std::cout << "ROOT          : " << server.config.root << std::endl;
        std::cout << "INDEX         : " << server.config.index << std::endl;
        std::cout << "ERROR_PAGE    : " << server.config.errorPage << std::endl;
        std::cout << "AUTOINDEX     : " << server.config.autoindex << std::endl;
        std::cout << "MAX_BODY_SIZE : " << server.config.maxBodySize << std::endl;
        std::cout << "CGI           : " << server.config.cgi << std::endl;
        std::cout << "METHODS       : " << server.config.allowedMethods << std::endl;
        std::cout << "REDIRECTION   : " << server.config.httpRedirection << std::endl;
        return 0;
    }
    else {
        for (int i = 0; i < (int)server.location.size(); i++) {
            if (request.getPath() == server.location[i].location) {
                validateLocation(request, server.location[i]);
                return 0;
            }
        }   
    }
    _status = 404;
    return 1;
}


void Response::validateLocation(Request& request, LocationConfig& location) {
    // validate the method
    if (location.config.allowedMethods.find(request.getMethod()) == std::string::npos) {
        _status = 405;
        return;
    }
    printLocation(location);
}
