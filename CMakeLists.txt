cmake_minimum_required(VERSION 3.16)
project(BlazeLog LANGUAGES CXX)

# ================================
# 🔧 Global Config
# ================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Show all warnings (useful for development)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif (MSVC)
    add_compile_options(/W4 /WX)
endif()

# ================================
# 📁 Project Structure
# ================================
# Library
add_library(blazelog
    src/Logger.cpp
    src/FileWriter.cpp
    src/ThreadSafeQueue.cpp
)

# Include directories
target_include_directories(blazelog
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# Optional: Enable position independent code for library use in shared libs
set_target_properties(blazelog PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ================================
# 🧩 Example Executable
# ================================
add_executable(blazelog_example examples/main.cpp)
target_link_libraries(blazelog_example PRIVATE blazelog)

# ================================
# 🧱 Install / Export (Optional)
# ================================
# Allows installing BlazeLog for reuse
install(TARGETS blazelog
    EXPORT BlazeLogTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

# ================================
# 🧪 Developer Info
# ================================
message(STATUS "========================================")
message(STATUS " BlazeLog Build Configuration")
message(STATUS "   C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "   Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "   Output Directory: ${CMAKE_BINARY_DIR}")
message(STATUS "========================================")
