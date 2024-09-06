cmake_minimum_required(VERSION 3.12)

# Project name and version
project(dc VERSION 1.0.6)

# Retrieve build date
string(TIMESTAMP BUILD_DATE "%d-%b-%Y %H:%M:%S")

# Retrieve git hash
execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE DC_GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)

# Set debug flags by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Debug build flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -Wall -Wextra -pedantic -Werror \
    -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
    -Wformat-security -fsanitize=address -fsanitize=undefined \
    -fstack-clash-protection -Wundef -Wshadow -Wpointer-arith \
    -Wcast-align -Wwrite-strings -ftrapv -std=c++20 -g -O2")

# Release build flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++20")

# Get build flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DC_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(DC_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# Get compiler ID and version
set(DC_COMPILER "${CMAKE_CXX_COMPILER_ID}")
set(DC_COMPILER_V "${CMAKE_CXX_COMPILER_VERSION}")

# Main file
add_executable(dc main.cpp)

# Source files
include_directories(src)
add_subdirectory(src)

# Link sources to main
target_link_libraries(dc src)

# Pass variables to the preprocessor
add_compile_definitions(DC_VER="${PROJECT_VERSION}")
add_compile_definitions(DC_HASH="${DC_GIT_HASH}")
add_compile_definitions(DC_BUILD="${CMAKE_BUILD_TYPE}")
add_compile_definitions(DC_FLAGS="${DC_FLAGS}")
add_compile_definitions(DC_BUILD_DATE="${BUILD_DATE}")
add_compile_definitions(DC_COMPILER="${DC_COMPILER}")
add_compile_definitions(DC_COMPILER_V="${DC_COMPILER_V}")