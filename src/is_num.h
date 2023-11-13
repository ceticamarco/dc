#pragma once
#include <sstream>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <typename T>
bool is_num(const std::string &str) requires Numeric<T> {
    std::istringstream ss(str);
    T number;

    return (ss >> number) && ss.eof();
}
