cmake_minimum_required(VERSION 3.12)
project(dc)

set(CMAKE_CXX_STANDARD 20)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} \
    -Wall -Wextra -pedantic -Werror \
    -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
    -Wformat-security -fsanitize=address -fsanitize=undefined \
    -fstack-clash-protection -Wundef -Wshadow -Wpointer-arith \
    -Wcast-align -Wwrite-strings -ftrapv -std=c++20 -g -O2")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++20")

add_executable(dc main.cpp)
include_directories(src)

add_subdirectory(src)

target_link_libraries(dc src)