# BlazeLog

A simple, header-only, thread-safe, asynchronous logging library for C++.

## Features

- **Asynchronous:** Logging calls are non-blocking.
- **Thread-safe:** Can be used from multiple threads.
- **Header-only:** Easy to integrate into any project.
- **Log levels:** Supports different log levels (Info, Warning, Error, Debug).
- **File-based:** Logs are written to a file.

## How to Build and Run

### Prerequisites

- C++20 compatible compiler (GCC, Clang, MSVC)
- CMake 3.16+

### Build Steps

1.  **Clone the repository:**
    ```sh
    git clone git@github.com:MishraMohit21/BlazeLog_Lib.git
    cd BlazeLog_Lib
    ```

2.  **Create a build directory:**
    ```sh
    mkdir build
    cd build
    ```

3.  **Configure and build:**
    ```sh
    cmake ..
    cmake --build .
    ```

4.  **Run the example:**
    ```sh
    ./blazelog_example
    ```

## Usage

1.  **Include the `Logger.h` header:**
    ```cpp
    #include "Logger.h"
    ```

2.  **Initialize the logger:**
    ```cpp
    blazelogger::Logger::Init("logs/output.log");
    ```

3.  **Use the logging macros:**
    ```cpp
    LOG_INFO("This is an info message.");
    LOG_WARNING("This is a warning message.");
    LOG_ERROR("This is an error message.");
    LOG_DEBUG("This is a debug message.");
    ```

4.  **Shutdown the logger:**
    ```cpp
    blazelogger::Logger::Shutdown();
    ```
