#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "webserv.hpp"
#include <iostream>
#include <sstream>

class Request {
private:
    std::string _method;
    std::string _path;
    std::string _version;
    std::string _host;
    std::string _port; 
    std::string _contentType;
    std::string _contentLength;
    std::string _body;

public:
    Request();
    ~Request();

    void parseRequest(std::string request);
    void printRequest();
    std::string makelog() {
        std::string log = "\"";
        log += _method + " ";
        log += _path + " ";
        log += _version + " ";
        log += _host + ":";
        log += _port + "\"";
        return log;
    }

    std::string getMethod() { return _method; }
    std::string getPath() { return _path; }
    std::string getVersion() { return _version; }
    std::string getHost() { return _host; }
    std::string getPort() { return _port; }
    std::string getContentType() { return _contentType; }
    std::string getContentLength() { return _contentLength; }
    std::string getBody() { return _body; }
};

#endif

// GET /path/to/resource HTTP/1.1
// Host: www.example.com
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive

// POST /path/to/resource HTTP/1.1
// Host: www.example.com
// User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3
// Content-Type: application/x-www-form-urlencoded
// Content-Length: 27
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive

// field1=value1&field2=value2