#include "Logger.hpp"

Logger::Logger() {}
Logger::~Logger() {}

void Logger::log(const std::string& message, Level level) {
    std::string levelStr;
    std::string colorCode;
    switch (level) {
        case INFO:
            levelStr = "[ INFO ]";
            colorCode = "\033[1;34m"; // Blue
            break;
        case WARNING:
            levelStr = "[ WARNING ]";
            colorCode = "\033[1;33m"; // Yellow
            break;
        case ERROR:
            levelStr = "[ ERROR ]";
            colorCode = "\033[1;31m"; // Red
            break;
    }

    std::string logMessage = getCurrentTime() + " " + levelStr + ": " + message;
    std::cout << colorCode << logMessage << "\033[0m" << std::endl; // Reset color
}

std::string Logger::getCurrentTime() {
    time_t now = time(0);
    struct tm * timeinfo = localtime(&now);
    char buffer[18];
    strftime(buffer, 18, "%y/%m/%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}
