#pragma once

#include <sstream>
#include <cmath>
#include <iomanip>

/**
 * @brief Constrains a generic type to integral/float type
 */
template <typename T>
concept numeric = std::is_arithmetic_v<T>;

/**
 * @brief General purpose numeric manipulation utilities
 * 
 * This class is **not** meant to be instantiated
 */
class NumericUtils {
public:
    NumericUtils() = delete;
    static std::string format_number(double number, unsigned int precision);
    template <typename T>
    requires numeric<T>
    static bool is_numeric(const std::string &str);
};

/**
 * @brief Determines whether a given string is a number or not
 * 
 * @param T The 
 * @param str A string containing either a numeric or a non-numeric value
 * 
 * @return true if the string contains a numeric value, false otherwise
 */
template <typename T>
requires numeric<T>
bool NumericUtils::is_numeric(const std::string &str) {
    std::istringstream ss(str);
    T number;

    return (ss >> number) && ss.eof();
}