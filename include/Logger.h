#ifndef BLAZELOG_LOGGER_H
#define BLAZELOG_LOGGER_H

#include <string>
#include <memory>
#include <thread>
#include "ThreadSafeQueue.h"
#include "FileWriter.h"

namespace blazelogger {

enum class LogLevel {
    Info,
    Warning,
    Error,
    Debug
};

struct LogMessage {
    LogLevel level;
    std::string message;
    std::string timestamp;
};

class Logger {
public:
    static void Init(const std::string& log_file_path);
    static void Log(LogLevel level, const std::string& message);
    static void Shutdown();
    Logger(const std::string& log_file_path);
    ~Logger();


private:

    void worker_thread_func();

    static std::unique_ptr<Logger> instance_;
    static std::mutex instance_mutex_;

    FileWriter file_writer_;
    blazeLogger::ThreadSafeQueue<LogMessage> log_queue_;
    std::thread worker_thread_;
    bool running_ = true;
};

} // namespace blazelogger

#define LOG_INFO(msg) blazelogger::Logger::Log(blazelogger::LogLevel::Info, msg)
#define LOG_WARNING(msg) blazelogger::Logger::Log(blazelogger::LogLevel::Warning, msg)
#define LOG_ERROR(msg) blazelogger::Logger::Log(blazelogger::LogLevel::Error, msg)
#define LOG_DEBUG(msg) blazelogger::Logger::Log(blazelogger::LogLevel::Debug, msg)

#endif // BLAZELOG_LOGGER_H
