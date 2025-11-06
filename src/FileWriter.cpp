#include "FileWriter.h"
#include <iostream>
#include <filesystem>


namespace blazelogger
{
    FileWriter::FileWriter(const std::string &path, bool flushEveryWrite, bool truncate)
    :flush_every_write_(flushEveryWrite), path_(path)
    {
        try {
            std::filesystem::path p(path);
            auto dir = p.parent_path();

            // Ensure parent directory exists
            if (!dir.empty() && !std::filesystem::exists(dir)) {
                std::filesystem::create_directories(dir);
            }

            // Open in append mode or truncate mode
            std::ios_base::openmode mode = std::ios::out;
            if (truncate) {
                mode |= std::ios::trunc;
            } else {
                mode |= std::ios::app;
            }
            file_.open(p, mode);

            if (!file_.is_open()) {
                std::cerr << "Failed to open log file: " << path << "\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error in Opening the file: " << e.what() << "\n";
        }
    }

    FileWriter::~FileWriter()
    {
        if (file_.is_open()) {
            file_.flush();
            file_.close();
        }
    }

    void FileWriter::write(std::string_view message)
    {

        if (!file_.is_open())
        {
            std::cerr << "Attempted to write to closed file: " << path_ << "\n";
            return;
        
        }
        
        std::lock_guard<std::mutex> lock(mutex_);

        file_ << message;

        if (!file_) 
        {
            std::cerr << "Failed to log the message to file: " << path_ << std::endl;
            return;
        }

        if(flush_every_write_)
        {
            file_.flush();
        }

    }

    void FileWriter::flush() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (file_.is_open()) {
            file_.flush();
        }
    }

} // namespace blazelogger
