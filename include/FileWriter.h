#pragma once
#include <string>
#include <fstream>
#include <mutex>


namespace blazelogger
{
    
    class FileWriter {
    public:
        /// @brief 
        /// @param path 
        /// @param flushEveryWrite 
        explicit FileWriter(const std::string& path, bool flushEveryWrite = false, bool truncate = false);
        ~FileWriter();

        void write(std::string_view message);
        void flush();

    private:
        std::ofstream file_;
        std::mutex mutex_;
        bool flush_every_write_;
        std::string path_;
    };

} // namespace blazelogger
