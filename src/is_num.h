#pragma once
#include <sstream>

/**
 * @brief Constrains a generic type to integral/float type
 */
template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

/**
 * @brief Determines if a string is a number of not
 * @param str A string containing either a numeric or a non-numeric value
 * @return true if the string contains a number, false otherwise
 */
template <typename T>
requires Numeric<T>
bool is_num(const std::string &str) {
    std::istringstream ss(str);
    T number;

    return (ss >> number) && ss.eof();
}
