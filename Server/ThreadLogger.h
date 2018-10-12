#pragma once

#include <string>

class ThreadLogger {
public:
    explicit ThreadLogger(std::string threadName);
    ~ThreadLogger();

    void info(std::string logMessage);
    void error(std::string logMessage);

private:
    std::string threadName;
};
