cmake_minimum_required(VERSION 3.12)
project(dc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
    -Wall -Wextra -Werror -pedantic-errors \
    -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
    -Wformat-security -fsanitize=address -fsanitize=undefined \
    -fstack-clash-protection -Wundef -Wshadow -Wpointer-arith \
    -Wcast-align -Wwrite-strings -ftrapv -std=c++20 -O3")

add_executable(dc main.cpp)
include_directories(src)
add_subdirectory(src)

target_link_libraries(dc src)
