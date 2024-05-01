#include "Response.hpp"


#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <sstream>

std::string generateDirectoryListing(const std::string& path) {
    DIR* dir;
    struct dirent* ent;
    std::ostringstream html;

    html << "<html><body><h1>Index of " << path << "</h1><ul>";

    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            html << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>";
        }
        closedir(dir);
    } else {
        // Could not open directory
        return "";
    }

    html << "</ul></body></html>";
    return html.str();
}


Response::Response() {}
Response::~Response() {}
Response::Response(Request& request, std::vector<ServerConfig>& servers) : _servers(servers) {
    _status = 200;
    // getConfig(request, servers);

    validateServer(request, servers);
}

void saveFileFromRequestBody(const std::string& requestBody) {
    std::istringstream requestStream(requestBody);

    std::string boundary;
    std::getline(requestStream, boundary);
    boundary = boundary.substr(0, boundary.size() - 1);

    std::string contentDisposition;
    std::getline(requestStream, contentDisposition);

    std::string fileName = contentDisposition.substr(contentDisposition.find("filename=") + 10);
    fileName = fileName.substr(0, fileName.size() - 2); // remove the trailing quote

    std::string contentType;
    std::getline(requestStream, contentType);

    std::string blankLine;
    std::getline(requestStream, blankLine);

    std::ofstream outputFile;
    std::string filePath = "./www/uploads/" + fileName;
    std::string fileExtension = fileName.substr(fileName.find_last_of(".") + 1);
    if (fileExtension == "txt" || fileExtension == "html" || fileExtension == "css" || fileExtension == "js") {
        outputFile.open(filePath.c_str());
    } else {
        outputFile.open(filePath.c_str(), std::ios::binary);
    }

    std::string body;
    std::getline(requestStream, body);

    size_t body_pos = requestBody.find("\r\n\r\n", boundary.size() + 2) + 4; // 4 is the length of "\r\n\r\n"
    size_t boundary_end_pos = requestBody.find(boundary + "--", body_pos);

    std::string fileContent = requestBody.substr(body_pos, boundary_end_pos - body_pos);
    outputFile.write(fileContent.c_str(), fileContent.size());

    outputFile.close();
}


std::string htmlEncode(const std::string& data) {
    std::string buffer;
    buffer.reserve(data.size());
    for(size_t pos = 0; pos != data.size(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.append("&amp;");       break;
            case '\"': buffer.append("&quot;");      break;
            case '\'': buffer.append("&apos;");      break;
            case '<':  buffer.append("&lt;");        break;
            case '>':  buffer.append("&gt;");        break;
            default:   buffer.append(&data[pos], 1); break;
        }
    }
    return buffer;
}


std::string readUploadedFile(const std::string& path) {
    std::ifstream file(path.c_str());
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return htmlEncode(content);
}


void Response::validateServer(Request& request, std::vector<ServerConfig>& servers) {
    if (servers.empty()) {
        _status = 500;
        return;
    }
    else if (request.getVersion() != "HTTP/1.1") {
        _status = 505;
        return;
    }

    for (size_t i = 0; i < servers.size(); i++) {
        // validate the port and host
        if (servers[i].config.listen == request.getPort() && servers[i].config.server_name == request.getHost()) {
            validatePath(request, servers[i]);
            return;
        }
    }
    _status = 502;
}


int Response::validatePath(Request& request, ServerConfig& server) {
    // validate the path
    if (request.getPath() == "/") {
        // std::cout << "PORT          : " << server.config.listen << std::endl;
        // std::cout << "SERVER_NAME   : " << server.config.server_name << std::endl;
        // std::cout << "ROOT          : " << server.config.root << std::endl;
        // std::cout << "INDEX         : " << server.config.index << std::endl;
        // std::cout << "ERROR_PAGE    : " << server.config.errorPage << std::endl;
        // std::cout << "AUTOINDEX     : " << server.config.autoindex << std::endl;
        // std::cout << "MAX_BODY_SIZE : " << server.config.maxBodySize << std::endl;
        // std::cout << "CGI           : " << server.config.cgi << std::endl;
        // std::cout << "METHODS       : " << server.config.allowedMethods << std::endl;
        // std::cout << "REDIRECTION   : " << server.config.httpRedirection << std::endl;
        _config = &server.config;
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
    // _status = 404;
    return 1;
}


void Response::validateLocation(Request& request, LocationConfig& location) {
    // validate the method
    if (location.config.allowedMethods.find(request.getMethod()) == std::string::npos) {
        _status = 405;
        return;
    }
    _config = &location.config;
    // printLocation(location);
}


std::string Response::makeResponse(Request& request) {
    std::string header = "\r\nContent-Type: text/html\r\nConnection: Close\r\nServer: WebServ\r\n\r\n";
    std::string body = "";
    
    if (_config->autoindex == "on") {
        body = generateDirectoryListing("./www" + request.getPath());
    }
    else if (_status == 502) {
        body = "<html><body><h1>502 Internal Server Error</h1></body></html>";
    }
    else if (_status == 200 && request.getMethod() == "GET") {
        std::string root = (_config->root == "") ? "./www" : _config->root;
        std::string path = root + "/" + _config->index;
        // std::cout << "Path: " << path << std::endl;
        // abre o arquivo de index
        // std::ifstream indexFile(path.c_str());
        // if (indexFile.good()) {
        //     // salva o conteudo do arquivo no body
        //     body = std::string((std::istreambuf_iterator<char>(indexFile)), std::istreambuf_iterator<char>());
        //     // body = "<html><body><h1>200 OK</h1> <form action='/uploads' method='post' enctype='multipart/form-data'> <input type='file' name='file'> <input type='submit' value='Upload'> </form> </body></html>";
        //     indexFile.close();
        // } else {
        //     _status = 404;
        //     indexFile.close();
        // }
        // body = "hello from make response";
        // envia um uma pagina html com um post de um arquivo txt/plain
        body += "<html><body><h1>200 OK</h1> <form action='#' method='post' enctype='multipart/form-data'>";
        body += "<input type='file' name='file'> <input type='submit' value='Upload'> </form> </body></html>";
    }
    else if (_status == 404) {
        // std::string errorPage = (_config->errorPage.empty()) ? _servers[0].config.errorPage : _config->errorPage;
        // std::ifstream file(errorPage.c_str());
        body = "<html><body><h1>404 Not Found</h1></body></html>";
        // body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        // std::cout << "body: " << body << std::endl;
    }
    else if (request.getMethod() == "POST") {
        // std::cout << "POST" << std::endl;
        saveFileFromRequestBody(request.getBody());
        // std::string file = readUploadedFile("./www/uploads/hello.txt");
        body = "<meta charset='UTF-8'><html><body><h1>201 OK</h1> <pre>";
        body += "<img src='www/uploads/1665095685245.jpg'>";
        // body += file;
        body += "</pre> </body></html>";
        _status = 201;
    }

    std::string response = "HTTP/1.1 " + numberToString(_status) + " " + statusCode.get(_status) + header + body + "\n";
    std::string logMessage = request.makelog() + " " + numberToString(_status) + " " + statusCode.get(_status);
    logger.log(logMessage, Logger::INFO);
    return response;
}
