#include "Logger.h"
#include <thread>
#include <iostream>
#include <string>

int main() {
    blazelogger::Logger::Init("logs/output.log");

    // Multiple threads writing logs
    std::thread t1([] {
        for (int i = 0; i < 3; ++i) {
            LOG_INFO("[Thread 1] Message " + std::to_string(i));
        }
    });

    std::thread t2([] {
        for (int i = 0; i < 3; ++i) {
            LOG_ERROR("[Thread 2] Message " + std::to_string(i));
        }
    });

    t1.join();
    t2.join();

    blazelogger::Logger::Shutdown();
    std::cout << "Logs written successfully.\n";

    return 0;
}