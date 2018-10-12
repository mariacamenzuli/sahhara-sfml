#include "ThreadLogger.h"
#include <iostream>

ThreadLogger::ThreadLogger(std::string threadName): threadName(std::move(threadName)) {
}

ThreadLogger::~ThreadLogger() = default;

void ThreadLogger::info(std::string logMessage) {
    std::cout << "[" << threadName << "] " << logMessage << std::endl;
}

void ThreadLogger::error(std::string logMessage) {
    std::cerr << "[" << threadName << "] " << logMessage << std::endl;
}
