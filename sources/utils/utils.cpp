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
