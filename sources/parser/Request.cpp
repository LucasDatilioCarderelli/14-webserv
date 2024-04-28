#include "Request.hpp"

Request::Request() {
    _method = "";
    _path = "";
    _version = "";
    _host = "";
    _port = "";
    _contentType = "";
    _contentLength = "";
    _body = "";
}

Request::~Request() {}

void Request::parseRequest(std::string request) {
    std::string line;
    std::istringstream requestStream(request);

    // Parse the first line of the request
    std::getline(requestStream, line);
    std::istringstream lineStream(line);
    lineStream >> _method >> _path >> _version;

    // Parse the headers of the request
    while (std::getline(requestStream, line)) {
        if (line.empty() || line == "\r") {
            break;
        }

        std::istringstream headerStream(line);
        std::string headerName;
        std::string headerValue;

        std::getline(headerStream, headerName, ':');
        headerStream >> headerValue;

        if (headerName == "Host") {
            std::istringstream hostStream(headerValue);
            std::getline(hostStream, _host, ':');
            hostStream >> _port;
        } else if (headerName == "Content-Type") {
            std::istringstream contentTypeStream(headerValue);
            std::getline(contentTypeStream, _contentType, ';');
            std::string remainingStream = headerStream.str().substr(headerStream.tellg());
            std::istringstream boundaryStream(remainingStream);
            std::string boundary;
            std::getline(boundaryStream, boundary, '=');
            std::getline(boundaryStream, _boundary);
            _boundary = _boundary.substr(0, _boundary.size() - 1);
        } else if (headerName == "Content-Length") {
            _contentLength = headerValue;
        }
    }

    // Parse the body of the request
    while (std::getline(requestStream, line)) {
        _body += line + "\n";
    }
}

void Request::printRequest() {
    std::cout << std::endl << std::endl;
    std::cout << "Method        : " << _method << std::endl;
    std::cout << "Path          : " << _path << std::endl;
    std::cout << "Version       : " << _version << std::endl;
    std::cout << "Host          : " << _host << std::endl;
    std::cout << "Port          : " << _port << std::endl;
    std::cout << "Content-Type  : " << _contentType << std::endl;
    std::cout << "Boundary      : " << _boundary << std::endl;
    std::cout << "Content-Length: " << _contentLength << std::endl;
    // std::cout << "Body          : " << _body << std::endl;
    std::cout << std::endl << std::endl;
}
