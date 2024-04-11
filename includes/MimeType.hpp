#ifndef MIMETYPE_HPP
#define MIMETYPE_HPP

#include <map>
#include <string>

class MimeType {
private:
    std::map<std::string, std::string> _mimeTypes;

public:
    MimeType();
    ~MimeType();
    std::string get(std::string extension);
};

#endif
