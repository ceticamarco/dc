#include "num_utils.h"

/**
 * @brief Formats a decimal number according to the precision parameter
 * 
 * @param number The number to be formatted
 * @param precision The precision to format the number
 * 
 * @return A string containing the formatted number
 */
std::string NumericUtils::format_number(double number, unsigned int precision) {
    std::ostringstream oss;

    // Preserve non-zero decimals even when precision is zero
    if(precision == 0 && std::fmod(number, 1.0) != 0.0) {
        precision = 2;
    }

    oss << std::fixed << std::setprecision(static_cast<int>(precision)) << number;
    std::string res = oss.str();

    return res;
}