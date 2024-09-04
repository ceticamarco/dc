#include <bitset>
#include <iomanip>
#include <cmath>

#include "adt.cpp"
#include "bitwise.h"
#include "is_num.h"

std::optional<std::string> Bitwise::exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, __attribute__((unused))  std::unordered_map<char, dc::Register> &regs) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        case OPType::BAND: err = fn_bitwise_and(stack, parameters); break;
        case OPType::BOR: err = fn_bitwise_or(stack, parameters); break;
        case OPType::BNOT: err = fn_bitwise_not(stack, parameters); break;
        case OPType::BXOR: err = fn_bitwise_xor(stack, parameters); break;
        case OPType::BSL: err = fn_bitwise_lshift(stack, parameters); break;
        case OPType::BSR: err = fn_bitwise_rshift(stack, parameters); break;
        default: break;
    }

    return err;
}

/**
 * @brief Evaluates a bitwise AND
 * 
 * Takes two values from the stack and computes bitwise AND between the second one popped
 * and the first one popped
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Bitwise::fn_bitwise_and(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'{' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size() - 1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        std::bitset<64> rhs{std::stoul(stack.pop(true))};
        std::bitset<64> lhs{std::stoul(stack.pop(true))};

        // Compute bitwise AND and push back the result
        std::bitset<64> result = (lhs & rhs);
        stack.push(trim_digits(result.to_ullong(), parameters.precision));
    } else {
        return "'{' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Evaluates a bitwise OR
 * 
 * Takes two values from the stack and computes bitwise OR between the second one popped
 * and the first one popped
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Bitwise::fn_bitwise_or(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'}' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size() - 1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        std::bitset<64> rhs{std::stoul(stack.pop(true))};
        std::bitset<64> lhs{std::stoul(stack.pop(true))};

        // Compute bitwise AND and push back the result
        std::bitset<64> result = (lhs | rhs);
        stack.push(trim_digits(result.to_ullong(), parameters.precision));
    } else {
        return "'}' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Evaluates a bitwise NOT
 * 
 * Takes one value from the stack and computes bitwise NOT(one's complement)
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Bitwise::fn_bitwise_not(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'l' requires one operand";
    }

    // Extract one entry from the stack
    auto x = stack.pop(false);
    auto is_x_num = is_num<double>(x);

    // Check whether popped value is a number
    if(is_x_num) {
        stack.copy_xyz();
        // Compute bitwise NOT 
        int result = ~std::stoi(stack.pop(true));
        stack.push(trim_digits(result, parameters.precision));
    } else {
        return "'l' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Evaluates a bitwise XOR
 * 
 * Takes two values from the stack and computes bitwise XOR between the second one popped
 * and the first one popped
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Bitwise::fn_bitwise_xor(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'L' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size() - 1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        std::bitset<64> rhs{std::stoul(stack.pop(true))};
        std::bitset<64> lhs{std::stoul(stack.pop(true))};

        // Compute bitwise AND and push back the result
        std::bitset<64> result = (lhs ^ rhs);
        stack.push(trim_digits(result.to_ullong(), parameters.precision));
    } else {
        return "'L' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Shifts a value to the left
 * 
 * Takes two values from the stack and shifts to the left the second one popped
 * by _n_ values where _n_ is the first one popped
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Bitwise::fn_bitwise_lshift(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'m' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size() - 1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        std::bitset<64> pos{std::stoul(stack.pop(true))};
        std::bitset<64> value{std::stoul(stack.pop(true))};

        // Compute bitwise left shift and push back the result
        std::bitset<64> result = (value << pos.to_ulong());
        stack.push(trim_digits(result.to_ullong(), parameters.precision));
    } else {
        return "'m' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Shifts a value to the right
 * 
 * Takes two values from the stack and shifts to the right the second one popped
 * by _n_ values where _n_ is the first one popped
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Bitwise::fn_bitwise_rshift(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'M' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size() - 1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        std::bitset<64> pos{std::stoul(stack.pop(true))};
        std::bitset<64> value{std::stoul(stack.pop(true))};

        // Compute bitwise right shift and push back the result
        std::bitset<64> result = (value >> pos.to_ulong());
        stack.push(trim_digits(result.to_ullong(), parameters.precision));
    } else {
        return "'M' requires numeric values";
    }

    return std::nullopt;
}

std::string Bitwise::trim_digits(double number, unsigned int precision) {
    std::ostringstream oss;

    // Preserve non-zero decimal numbers even when precision is zero
    if(precision == 0 && std::fmod(number, 1.0) != 0.0) {
        precision = 2;
    }

    oss << std::fixed << std::setprecision(static_cast<int>(precision)) << number;
    std::string s = oss.str();

    return s;
}
