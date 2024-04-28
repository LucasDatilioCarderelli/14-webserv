#include "Request.hpp"

Request::~Request() {}
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

void Request::parseRequest(std::string request) {
    std::istringstream requestStream(request);

    parseRequestLine(requestStream);
    parseHeaders(requestStream);
    parseBody(requestStream);
}

void Request::parseRequestLine(std::istringstream& requestStream) {
    std::string line;
    std::getline(requestStream, line);
    std::istringstream lineStream(line);
    lineStream >> _method >> _path >> _version;
}

void Request::parseHeaders(std::istringstream& requestStream) {
    std::string line;
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
            parseHostHeader(headerValue);
        } else if (headerName == "Content-Type") {
            parseContentTypeHeader(headerValue, headerStream);
        } else if (headerName == "Content-Length") {
            parseContentLengthHeader(headerValue);
        }
    }
}

void Request::parseBody(std::istringstream& requestStream) {
    std::string line;
    while (std::getline(requestStream, line)) {
        _body += line + "\n";
    }
}

void Request::parseHostHeader(const std::string& headerValue) {
    std::istringstream hostStream(headerValue);
    std::getline(hostStream, _host, ':');
    hostStream >> _port;
}

void Request::parseContentTypeHeader(const std::string& headerValue, std::istringstream& headerStream) {
    std::istringstream contentTypeStream(headerValue);
    std::getline(contentTypeStream, _contentType, ';');
    std::string remainingStream = headerStream.str().substr(headerStream.tellg());
    std::istringstream boundaryStream(remainingStream);
    std::string boundary;
    std::getline(boundaryStream, boundary, '=');
    std::getline(boundaryStream, _boundary);
    _boundary = _boundary.substr(0, _boundary.size() - 1);
}

void Request::parseContentLengthHeader(const std::string& headerValue) {
    _contentLength = headerValue;
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
    std::cout << "Body          : " << (_body.empty() ? "empty" : "...") << std::endl;
    std::cout << std::endl << std::endl;
}

std::string Request::makelog() {
    std::string log = "\"";
    log += _method + " ";
    log += _path + " ";
    log += _version + " ";
    log += _host + ":";
    log += _port + "\"";
    return log;
}
