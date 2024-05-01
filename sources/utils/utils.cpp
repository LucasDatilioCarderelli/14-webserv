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
    std::string trimmedLine = line;
    trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));
    trimmedLine.erase(trimmedLine.find_last_not_of(" \t") + 1);
    return trimmedLine;
}
