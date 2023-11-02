#pragma once
#include <sstream>

template <typename T>
bool is_num(const std::string &str) {
    std::istringstream ss(str);
    T number;

    return (ss >> number) && ss.eof();
}
