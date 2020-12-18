// profiler

#pragma once

#include <chrono>
#include <iostream>
#include <string>

// macros provides easy access to profiler
#define LOG_DURATION(x) LogDuration logger(x)

// profiler class
class LogDuration { // measure execution time of a scope
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::string caption;

public:
    // remember start time
    LogDuration() : start(std::chrono::high_resolution_clock::now()) {}

    // and name of the scope optionally
    LogDuration(const std::string& caption) : start(std::chrono::high_resolution_clock::now()),
        caption(caption) {}

    ~LogDuration() { // output time and caption
        std::cout << caption << " " << (std::chrono::high_resolution_clock::now() - start).count() / 1000000 << " ms" << std::endl;
    }
};
