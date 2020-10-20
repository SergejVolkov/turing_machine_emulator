#pragma once

#include <chrono>
#include <iostream>
#include <string>

#define LOG_DURATION(x) LogDuration logger(x)

class LogDuration {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::string caption;

public:
    LogDuration() : start(std::chrono::high_resolution_clock::now()) {}

    LogDuration(const std::string& caption) : start(std::chrono::high_resolution_clock::now()),
        caption(caption) {}

    ~LogDuration() {
        std::cout << caption << " " << (std::chrono::high_resolution_clock::now() - start).count() / 1000000 << " ms" << std::endl;
    }
};
