#include "Response.hpp"


#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <sstream>


Response::Response() {}
Response::~Response() {}
Response::Response(Request& request, std::vector<ServerConfig>& servers) {
    _status = 200;
    // getConfig(request, servers);

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
    if (request.getVersion() != "HTTP/1.1") {
        setStatus(505); // HTTP Version Not Supported
        return;
    }

    if (request.getMethod() != "GET" && request.getMethod() != "POST" && request.getMethod() != "DELETE") {
        setStatus(501); // Not Implemented
        return;
    }

    // get the server config
    ServerConfig* server = NULL;
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].listen == request.getPort()) {
            server = &servers[i];
        }
    }
    // printServer(*server, false);

    // validate the server
    if (server->server_name != request.getHost()) {
        setStatus(502); // Bad Gateway
        return;
    }

    // validate the path
    _config = getConfig(request, *server);
    // printConfig(*_config);

    // validate the method
    if (_config->allowedMethods.find(request.getMethod()) == std::string::npos) {
        _status = 405; // Method Not Allowed
        return;
    }

    // request.printRequest();
    if (stringToNumber(request.getContentLength()) > stringToNumber(_config->maxBodySize)) {
        _status = 413; // Payload Too Large
        return;
    }
}

std::string Response::makeResponse(Request& request) {
    std::string header = "\r\nContent-Type: text/html\r\nConnection: Close\r\nServer: WebServ\r\n\r\n";
    std::string body = "<html><body><h1>200 OK</h1> <p> Hello world </p> </body></html>";

    std::string response = "HTTP/1.1 " + numberToString(_status) + " " + statusCode.get(_status) + header + body + "\n";
    std::string logMessage = request.makelog() + " " + numberToString(_status) + " " + statusCode.get(_status);
    logger.log(logMessage, Logger::INFO);
    return response; 
}



    // for (size_t i = 0; i < servers.size(); i++) {
    //     if (servers[i].config.listen == request.getPort() && servers[i].config.server_name == request.getHost()) {
    //         validatePath(request, servers[i]);
    //         return;
    //     }
    // }


// int Response::validatePath(Request& request, ServerConfig& server) {
//     // validate the path
//     if (request.getPath() == "/") {
//         // std::cout << "PORT          : " << server.config.listen << std::endl;
//         // std::cout << "SERVER_NAME   : " << server.config.server_name << std::endl;
//         // std::cout << "ROOT          : " << server.config.root << std::endl;
//         // std::cout << "INDEX         : " << server.config.index << std::endl;
//         // std::cout << "ERROR_PAGE    : " << server.config.errorPage << std::endl;
//         // std::cout << "AUTOINDEX     : " << server.config.autoindex << std::endl;
//         // std::cout << "MAX_BODY_SIZE : " << server.config.maxBodySize << std::endl;
//         // std::cout << "CGI           : " << server.config.cgi << std::endl;
//         // std::cout << "METHODS       : " << server.config.allowedMethods << std::endl;
//         // std::cout << "REDIRECTION   : " << server.config.httpRedirection << std::endl;
//         _config = &server.config;
//         return 0;
//     }
//     else {
//         for (int i = 0; i < (int)server.location.size(); i++) {
//             if (request.getPath() == server.location[i].location) {
//                 validateLocation(request, server.location[i]);
//                 return 0;
//             }
//         }   
//     }
//     // _status = 404;
//     return 1;
// }


// void Response::validateLocation(Request& request, LocationConfig& location) {
//     // validate the method
//     if (location.config.allowedMethods.find(request.getMethod()) == std::string::npos) {
//         _status = 405;
//         return;
//     }
//     _config = &location.config;
//     // printLocation(location);
// }


// std::string Response::makeResponse(Request& request) {
//     std::string header = "\r\nContent-Type: text/html\r\nConnection: Close\r\nServer: WebServ\r\n\r\n";
//     std::string body = "";
    
//     if (_config->autoindex == "on") {
//         body = generateDirectoryListing("./www" + request.getPath());
//     }
//     else if (_status == 502) {
//         body = "<html><body><h1>502 Internal Server Error</h1></body></html>";
//     }
//     else if (_status == 200 && request.getMethod() == "GET") {
//         std::string root = (_config->root == "") ? "./www" : _config->root;
//         std::string path = root + "/" + _config->index;
//         // std::cout << "Path: " << path << std::endl;
//         // abre o arquivo de index
//         // std::ifstream indexFile(path.c_str());
//         // if (indexFile.good()) {
//         //     // salva o conteudo do arquivo no body
//         //     body = std::string((std::istreambuf_iterator<char>(indexFile)), std::istreambuf_iterator<char>());
//         //     // body = "<html><body><h1>200 OK</h1> <form action='/uploads' method='post' enctype='multipart/form-data'> <input type='file' name='file'> <input type='submit' value='Upload'> </form> </body></html>";
//         //     indexFile.close();
//         // } else {
//         //     _status = 404;
//         //     indexFile.close();
//         // }
//         // body = "hello from make response";
//         // envia um uma pagina html com um post de um arquivo txt/plain
//         body += "<html><body><h1>200 OK</h1> <form action='#' method='post' enctype='multipart/form-data'>";
//         body += "<input type='file' name='file'> <input type='submit' value='Upload'> </form> </body></html>";
//     }
//     else if (_status == 404) {
//         // std::string errorPage = (_config->errorPage.empty()) ? _servers[0].config.errorPage : _config->errorPage;
//         // std::ifstream file(errorPage.c_str());
//         body = "<html><body><h1>404 Not Found</h1></body></html>";
//         // body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//         // std::cout << "body: " << body << std::endl;
//     }
//     else if (request.getMethod() == "POST") {
//         // std::cout << "POST" << std::endl;
//         saveFileFromRequestBody(request.getBody());
//         // std::string file = readUploadedFile("./www/uploads/hello.txt");
//         body = "<meta charset='UTF-8'><html><body><h1>201 OK</h1> <pre>";
//         body += "<img src='www/uploads/1665095685245.jpg'>";
//         // body += file;
//         body += "</pre> </body></html>";
//         _status = 201;
//     }

//     std::string response = "HTTP/1.1 " + numberToString(_status) + " " + statusCode.get(_status) + header + body + "\n";
//     std::string logMessage = request.makelog() + " " + numberToString(_status) + " " + statusCode.get(_status);
//     logger.log(logMessage, Logger::INFO);
//     return response;
// }
