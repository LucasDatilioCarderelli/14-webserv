#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <ctime>


class Logger {
public:
    enum Level {
        INFO,
        WARNING,
        ERROR
    };

    Logger();
    ~Logger();

    void log(const std::string& message, Level level = INFO);

private:
    std::string getCurrentTime();
};

#endif
