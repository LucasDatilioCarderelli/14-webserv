#include "Response.hpp"


std::string Response::convertPercentTwenty(const std::string& input) {
    std::string output = input;
    size_t pos = output.find("%20");
    while (pos != std::string::npos) {
        output.replace(pos, 3, " ");
        pos = output.find("%20", pos + 1);
    }
    return output;
}

bool Response::isFile(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISREG(buffer.st_mode);
}

bool Response::isDirectory(const std::string& path) {
    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        return false;
    }
    return S_ISDIR(buffer.st_mode);
}

std::string Response::getPath(Request& request) {
    std::string fullPath = _config->root + request.getPath();
    if (isDirectory(fullPath)) {
        if (fullPath[fullPath.size() - 1] != '/') {
            fullPath += "/";
        }
    }
    return fullPath;
}

// std::string Response::htmlEncode(const std::string& data) {
//     std::string buffer;
//     buffer.reserve(data.size());
//     for(size_t pos = 0; pos != data.size(); ++pos) {
//         switch(data[pos]) {
//             case '&':  buffer.append("&amp;");       break;
//             case '\"': buffer.append("&quot;");      break;
//             case '\'': buffer.append("&apos;");      break;
//             case '<':  buffer.append("&lt;");        break;
//             case '>':  buffer.append("&gt;");        break;
//             default:   buffer.append(&data[pos], 1); break;
//         }
//     }
//     return buffer;
// }
