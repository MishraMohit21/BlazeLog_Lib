#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace blazelogger {

std::unique_ptr<Logger> Logger::instance_ = nullptr;
std::mutex Logger::instance_mutex_;

std::string level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Info: return "[INFO]";
        case LogLevel::Warning: return "[WARNING]";
        case LogLevel::Error: return "[ERROR]";
        case LogLevel::Debug: return "[DEBUG]";
    }
    return "";
}

std::string format_message(const LogMessage& msg) {
    std::ostringstream oss;
    oss << msg.timestamp << " " << level_to_string(msg.level) << " " << msg.message << "\n";
    return oss.str();
}

Logger::Logger(const std::string& log_file_path) : file_writer_(log_file_path, false, true) {
    worker_thread_ = std::thread(&Logger::worker_thread_func, this);
}

Logger::~Logger() {
    if (running_) {
        Shutdown();
    }
}

void Logger::Init(const std::string& log_file_path) {
    std::lock_guard<std::mutex> lock(instance_mutex_);
    if (!instance_) {
        instance_ = std::unique_ptr<Logger>(new Logger(log_file_path));
    }
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (!instance_) {
        std::cerr << "Logger not initialized!" << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    instance_->log_queue_.push({level, message, oss.str()});
}

void Logger::Shutdown() {
    if (instance_) {
        instance_->running_ = false;
        instance_->log_queue_.shutdown();
        if (instance_->worker_thread_.joinable()) {
            instance_->worker_thread_.join();
        }
        instance_.reset();
    }
}

void Logger::worker_thread_func() {
    LogMessage msg;
    while (log_queue_.pop(msg)) {
        file_writer_.write(format_message(msg));
    }
    file_writer_.flush();
}

} // namespace blazelogger
