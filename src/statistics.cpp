#include <cmath>
#include <iomanip>

#include "adt.cpp"
#include "statistics.h"
#include "is_num.h"

std::optional<std::string> Statistics::exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        case OPType::PERM: err = fn_perm(stack, parameters); break;
        case OPType::COMB: err = fn_comb(stack, parameters); break;
        case OPType::SUMX: err = fn_sum(stack, parameters, regs); break;
        case OPType::SUMXX: err = fn_sum_squared(stack, parameters, regs); break;
        case OPType::MEAN: err = fn_mean(stack, parameters, regs); break;
        case OPType::SDEV: err = fn_sdev(stack, parameters, regs); break;
        case OPType::LREG: err = fn_lreg(stack, parameters, regs); break;
        default: break;
    }

    return err;
}

std::optional<std::string> Statistics::fn_perm(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'gP' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto head = stack[len];
    auto second = stack[len-1];
    auto is_head_num = is_num<long long>(head);
    auto is_second_num = is_num<long long>(second);

    // Check whether both entries are integers
    if(is_head_num && is_second_num) {
        stack.copy_xyz();
        auto x = std::stoll(stack.pop(true));
        auto y = std::stoll(stack.pop(true));

        // Compute factorial of numerator and denominator
        // and check if result is a non-negative integer
        auto numerator_opt = factorial(y);
        auto denominator_opt = factorial(y - x);
        if(numerator_opt == std::nullopt || denominator_opt == std::nullopt) {
            return "'gP' requires positive integers";
        }

        unsigned long long permutation = numerator_opt.value() / denominator_opt.value();
        stack.push(trim_digits(permutation, parameters.precision));
    } else {
        return "'gP' requires integer values";
    }

    return std::nullopt;
}

std::optional<std::string> Statistics::fn_comb(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'gC' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto head = stack[len];
    auto second = stack[len-1];
    auto is_head_num = is_num<long long>(head);
    auto is_second_num = is_num<long long>(second);

    // Check whether both entries are integers
    if(is_head_num && is_second_num) {
        stack.copy_xyz();
        auto n = std::stoull(stack.pop(true));
        auto k = std::stoull(stack.pop(true));

        // Check if combination is non-negative
        if(n > k) {
            return "'gC' requires positive integers";
        }

        // From Knuth's "Seminumerical Algorithms" book
        unsigned long long combination = 1;
        for(unsigned long long i = 1; i <= n; i++) {
            combination *= k--;
            combination /= i;
        }

        stack.push(trim_digits(combination, parameters.precision));
    } else {
        return "'gC' requires integer values";
    }

    return std::nullopt;
}

std::optional<std::string> Statistics::fn_sum(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether 'x' register exists
    if(regs.find('X') == regs.end()) {
        return "Register 'X' is undefined";
    }

    // Check if register's stack is empty
    if(regs['X'].stack.empty()) {
        return "The stack of register 'X' is empty";
    }

    // Othewise retrieve summation of register's stack
    auto summation = regs['X'].stack.summation();
    stack.push(trim_digits(summation, parameters.precision));

    return std::nullopt;
}

std::optional<std::string> Statistics::fn_sum_squared(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether 'x' register exists
    if(regs.find('X') == regs.end()) {
        return "Register 'X' is undefined";
    }

    // Check if register's stack is empty
    if(regs['X'].stack.empty()) {
        return "The stack of register 'X' is empty";
    }

    // Othewise retrieve summation of squares of register's stack
    auto summation_squared = regs['X'].stack.summation_squared();
    stack.push(trim_digits(summation_squared, parameters.precision));

    return std::nullopt;
}

std::optional<std::string> Statistics::fn_mean(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether 'x' register exists
    if(regs.find('X') == regs.end()) {
        return "Register 'X' is undefined";
    }

    // Check if register's stack is empty
    if(regs['X'].stack.empty()) {
        return "The stack of register 'X' is empty";
    }

    // Othewise compute mean
    auto summation = regs['X'].stack.summation();
    auto size = regs['X'].stack.size();
    auto mean = summation / size;
    stack.push(trim_digits(mean, parameters.precision));

    return std::nullopt;
}

std::optional<std::string> Statistics::fn_sdev(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether 'x' register exists
    if(regs.find('X') == regs.end()) {
        return "Register 'X' is undefined";
    }

    // Check if register's stack is empty
    if(regs['X'].stack.empty()) {
        return "The stack of register 'X' is empty";
    }

    // Othewise, compute the mean
    auto summation = regs['X'].stack.summation();
    auto count = regs['X'].stack.size();
    auto mean = summation / count;

    // Then compute the sum of the deviations from the mean and square the result
    const auto& const_vec = regs['X'].stack.get_ref();
    double sum_of_deviations = std::accumulate(const_vec.begin(), const_vec.end(), 0.0, 
        [&](double acc, const std::string& val) {
            double deviation = std::stod(val) - mean;
            return acc + std::pow(deviation, 2);
    });
    // Then compute the mean of previos values(variance)
    auto variance = sum_of_deviations / (count-1);
    
    // Finally, compute the square root of the variance(standard deviation)
    auto s_dev = sqrt(variance);

    stack.push(trim_digits(s_dev, parameters.precision));
    return std::nullopt;
}

std::optional<std::string> Statistics::fn_lreg(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
     // Check whether 'x' register exists
    if(regs.find('X') == regs.end()) {
        return "Register 'X' is undefined";
    }

    // Check if register's stack is empty
    if(regs['X'].stack.empty()) {
        return "The stack of register 'X' is empty";
    }

     // Check whether 'y' register exists
    if(regs.find('Y') == regs.end()) {
        return "Register 'Y' is undefined";
    }

    // Check if register's stack is empty
    if(regs['Y'].stack.empty()) {
        return "The stack of register 'Y' is empty";
    }

    // Check that both registers have the same length
    if(regs['X'].stack.size() != regs['Y'].stack.size()) {
        return "'X' and 'Y' registers must be of the same length";
    }

    // Othwerise, retrieve count and summations of both sets
    auto count = regs['X'].stack.size();
    auto x_sum = regs['X'].stack.summation();
    auto y_sum = regs['Y'].stack.summation();

    // Then compute the sum of products
    const auto& x_ref = regs['X'].stack.get_ref();
    const auto& y_ref = regs['Y'].stack.get_ref();
    std::size_t idx = 0;
    double sum_of_products = 0.0;

    for(auto it : x_ref) {
        auto x = std::stod(it);
        auto y = std::stod(y_ref[idx++]);
        sum_of_products += (x * y);
    }

    // Then compute the sum of squares
    auto x_sum_squares = regs['X'].stack.summation_squared();

    // Then compute the slope of the line(m)
    auto slope_numerator = ((count * sum_of_products) - (x_sum * y_sum));
    auto slope_denominator = ((count * x_sum_squares) - std::pow(x_sum, 2));
    auto slope = slope_numerator / slope_denominator;

    // Then compute the intercept of the line(b)
    auto intercept = (y_sum - (slope * x_sum)) / count;

    // Finally push the slope and the intercept(in this order) into the main stack
    stack.push(trim_digits(slope, parameters.precision));
    stack.push(trim_digits(intercept, parameters.precision));

    return std::nullopt;
}

std::optional<unsigned long long> Statistics::factorial(const long long n) {
    if(n < 0) {
        return std::nullopt;
    }

    unsigned long long factorial = 1;

    for(long long i = 2; i <= n; i++) {
        factorial *= i;
    }

    return factorial;
}

std::string Statistics::trim_digits(double number, unsigned int precision) {
    std::ostringstream oss;

    // Preserve non-zero decimal numbers even when precision is zero
    if(precision == 0 && std::fmod(number, 1.0) != 0.0) {
        precision = 2;
    }

    oss << std::fixed << std::setprecision(static_cast<int>(precision)) << number;
    std::string s = oss.str();

    return s;
}
