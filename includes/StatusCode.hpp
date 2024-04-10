#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <map>
#include <string>

class StatusCode {
private:
    std::map<int, std::string> _statusCodes;

public:
    StatusCode();
    ~StatusCode();
    std::string get(int code);
};

#endif
