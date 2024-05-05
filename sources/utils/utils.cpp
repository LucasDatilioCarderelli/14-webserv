#include "utils.hpp"
#include <sstream>

std::string numberToString(int Number) {
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

int stringToNumber(std::string str) {
    std::istringstream ss(str);
    int number;
    ss >> number;
    return number;
}

std::string trimLine(const std::string& line) {
    size_t first = line.find_first_not_of(' ');
    if (std::string::npos == first) {
        return line;
    }
    size_t last = line.find_last_not_of(' ');
    std::string trimmed = line.substr(first, (last - first + 1));

    // Remover espaços em branco extras no meio da string
    std::istringstream iss(trimmed);
    std::string result, word;
    while (iss >> word) {
        if (!result.empty()) {
            result += " ";
        }
        result += word;
    }
    return result;
}
