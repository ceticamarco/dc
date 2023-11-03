#include <cmath>

#include "math.h"
#include "is_num.h"

std::optional<std::string> Math::exec(stack_t &stack) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        case OPType::ADD: err = fn_add(stack); break;
        case OPType::SUB: err = fn_sub(stack); break;
        case OPType::MUL: err = fn_mul(stack); break;
        case OPType::DIV: err = fn_div(stack); break;
        case OPType::MOD: err = fn_mod(stack); break;
        case OPType::DIV_MOD: err = fn_div_mod(stack); break;
        case OPType::MOD_EXP: err = fn_mod_exp(stack); break;
        case OPType::EXP: err = fn_exp(stack); break;
        case OPType::SQRT: err = fn_sqrt(stack); break;
        case OPType::SIN: err = fn_sin(stack); break;
        case OPType::COS: err = fn_cos(stack); break;
        case OPType::TAN: err = fn_tan(stack); break;
        case OPType::FACT: err = fn_fact(stack); break;
        case OPType::PI: err = fn_pi(stack); break;
        case OPType::E: err = fn_e(stack); break;
        default: break;
    }

    return err;
}

std::optional<std::string> Math::fn_add(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'+' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto lhs = std::stod(stack.back());
        stack.pop_back();

        auto rhs = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(lhs + rhs));
    } else {
        return "'+' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sub(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'-' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto lhs = std::stod(stack.back());
        stack.pop_back();

        auto rhs = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(-(lhs - rhs)));
    } else {
        return "'-' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mul(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'*' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto lhs = std::stod(stack.back());
        stack.pop_back();

        auto rhs = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(lhs * rhs));
    } else {
        return "'*' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_div(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'/' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto divisor = std::stod(stack.back());
        stack.pop_back();

        auto dividend = std::stod(stack.back());
        stack.pop_back();

        // Check whether divisor is equal to zero
        if(divisor == (double)0) {
            return "Cannot divide by zero";
        }

        // Push back the result as a string
        stack.push_back(trim_zeros(dividend / divisor));
    } else {
        return "'/' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mod(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'%' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto rhs = std::stod(stack.back());
        stack.pop_back();

        auto lhs = std::stod(stack.back());
        stack.pop_back();

        // Check whether divisor is equal to zero
        if(rhs == (double)0) {
            return "Cannot divide by zero";
        }

        // Push back the result as a string
        stack.push_back(trim_zeros((int)lhs % (int)rhs));
    } else {
        return "'%' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_div_mod(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'~' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto divisor = std::stod(stack.back());
        stack.pop_back();

        auto dividend = std::stod(stack.back());
        stack.pop_back();

        // Check if divisor is not equal to zero
        if(divisor != (double)0) {
            auto quotient = std::trunc(dividend / divisor);
            auto remainder = ((int)dividend % (int)divisor);

            stack.push_back(trim_zeros(quotient));
            stack.push_back(trim_zeros(remainder));
        }

    } else {
        return "'~' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mod_exp(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 3) {
        return "'|' requires three operands";
    }

    // Otherwise extract three elements from the stack.
	// The first one is the modulus(n), the second one
	// is the exponent(e) and the third one is the base(b)
    auto len = stack.size()-1;
    auto n = stack.at(len);
    auto e = stack.at(len-1);
    auto b = stack.at(len-2);
    auto is_n_num = is_num<double>(n);
    auto is_e_num = is_num<double>(e);
    auto is_b_num = is_num<double>(b);

    // This functions computes
	// 		c ≡ b^e (mod n)
    if(is_n_num && is_e_num && is_b_num) {
        auto modulus = std::stoi(stack.back());
        stack.pop_back();

        auto exponent = std::stoi(stack.back());
        stack.pop_back();

        auto base = std::stoi(stack.back());
        stack.pop_back();

        if(modulus == 1) {
            stack.push_back("0");
            return std::nullopt;
        } else if(modulus == 0) {
            return "Modulus cannot be zero";
        }

        if(exponent < 0) {
            return "Exponent cannot be negative";
        }

        auto c = 1;
        for(auto i = 0; i < exponent; i++) {
            c = (c * base) % modulus;
        }
        
        stack.push_back(trim_zeros(c));
    } else {
        return "'|' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_exp(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'^' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        auto exp = std::stod(stack.back());
        stack.pop_back();

        auto base = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(pow(base, exp)));
    } else {
        return "'^' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sqrt(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'v' requires one operand";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto x = std::stod(stack.back());
        stack.pop_back();

        if(x <= (double)0) {
            return "'v' domain error";
        }

        // Push back the result as a string
        stack.push_back(trim_zeros(sqrt(x)));
    } else {
        return "'v' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sin(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'sin' requires one operand";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto x = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(sin(x)));
    } else {
        return "'sin' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_cos(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'cos' requires one operand";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto x = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(cos(x)));
    } else {
        return "'cos' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_tan(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'tan' requires one operand";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto x = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_zeros(tan(x)));
    } else {
        return "'tan' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_fact(stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'!' requires one operand";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        unsigned long factorial = 1;
        auto x = std::stod(stack.back());
        stack.pop_back();

        if(x == (double)0) {
            factorial = 1;
        }

        for(size_t i = 2; i <= (size_t)x; i++) {
            factorial *= i;
        }

        // Push back the result as a string
        stack.push_back(trim_zeros(factorial));
    } else {
        return "'!' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_pi(stack_t &stack) {
    stack.push_back(trim_zeros(std::numbers::pi));

    return std::nullopt;
}

std::optional<std::string> Math::fn_e(stack_t &stack) {
    stack.push_back(trim_zeros(std::numbers::e));

    return std::nullopt;
}

std::string Math::trim_zeros(double number) {
    std::string s = std::to_string(number);

    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
    s.erase(s.find_last_not_of('.') + 1, std::string::npos);

    return s;
}
