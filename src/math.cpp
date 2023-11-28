#include <cmath>
#include <numbers>
#include <iomanip>

#include "math.h"
#include "is_num.h"

std::optional<std::string> Math::exec(dc_stack_t &stack) {
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
        case OPType::ASIN: err = fn_asin(stack); break;
        case OPType::ACOS: err = fn_acos(stack); break;
        case OPType::ATAN: err = fn_atan(stack); break;
        case OPType::FACT: err = fn_fact(stack); break;
        case OPType::PI: err = fn_pi(stack); break;
        case OPType::E: err = fn_e(stack); break;
        default: break;
    }

    return err;
}

std::optional<std::string> Math::fn_add(dc_stack_t &stack) {
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
        stack.push_back(trim_digits((lhs + rhs), this->precision));
    } else {
        return "'+' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sub(dc_stack_t &stack) {
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

        // Subtract the two operands
        auto result = (-(lhs - rhs));

        // Compare the result with an epsilon value
        // to prevent -0/+0 results
        auto epsilon = 1e-10;
        if(std::abs(result) < epsilon) {
            result = 0.0;
        }

        // Push back the result as a string
        stack.push_back(trim_digits(result, this->precision));
    } else {
        return "'-' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mul(dc_stack_t &stack) {
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
        stack.push_back(trim_digits((lhs * rhs), this->precision));
    } else {
        return "'*' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_div(dc_stack_t &stack) {
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
        if(divisor == 0.0) {
            return "Cannot divide by zero";
        }

        // Push back the result as a string
        stack.push_back(trim_digits((dividend / divisor), this->precision));
    } else {
        return "'/' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mod(dc_stack_t &stack) {
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
        if(rhs == 0.0) {
            return "Cannot divide by zero";
        }

        // Push back the result as a string
        stack.push_back(trim_digits((static_cast<int>(lhs) % static_cast<int>(rhs)), this->precision));
    } else {
        return "'%' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_div_mod(dc_stack_t &stack) {
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
        if(divisor != 0.0) {
            auto quotient = std::trunc(dividend / divisor);
            auto remainder = ((int)dividend % (int)divisor);

            stack.push_back(trim_digits(quotient, this->precision));
            stack.push_back(trim_digits(remainder, this->precision));
        }

    } else {
        return "'~' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mod_exp(dc_stack_t &stack) {
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
        
        stack.push_back(trim_digits(c, this->precision));
    } else {
        return "'|' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_exp(dc_stack_t &stack) {
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
        stack.push_back(trim_digits(pow(base, exp), this->precision));
    } else {
        return "'^' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sqrt(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'v' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        if(val <= 0.0) {
            return "'v' domain error";
        }

        // Push back the result as a string
        stack.push_back(trim_digits(sqrt(val), this->precision));
    } else {
        return "'v' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sin(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'sin' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_digits(sin(val), this->precision));
    } else {
        return "'sin' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_cos(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'cos' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_digits(cos(val), this->precision));
    } else {
        return "'cos' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_tan(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'tan' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_digits(tan(val), this->precision));
    } else {
        return "'tan' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_asin(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'asin' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_digits(asin(val), this->precision));
    } else {
        return "'asin' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_acos(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'acos' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_digits(acos(val), this->precision));
    } else {
        return "'acos' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_atan(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'atan' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        auto val = std::stod(stack.back());
        stack.pop_back();

        // Push back the result as a string
        stack.push_back(trim_digits(atan(val), this->precision));
    } else {
        return "'atan' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_fact(dc_stack_t &stack) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'!' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        unsigned long factorial = 1;
        auto val = std::stod(stack.back());
        stack.pop_back();

        if(val == 0.0) {
            factorial = 1;
        }

        for(size_t i = 2; i <= (size_t)val; i++) {
            factorial *= i;
        }

        // Push back the result as a string
        stack.push_back(trim_digits(factorial, this->precision));
    } else {
        return "'!' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_pi(dc_stack_t &stack) {
    stack.push_back(trim_digits(std::numbers::pi, this->precision));

    return std::nullopt;
}

std::optional<std::string> Math::fn_e(dc_stack_t &stack) {
    stack.push_back(trim_digits(std::numbers::e, this->precision));

    return std::nullopt;
}

std::string Math::trim_digits(double number, unsigned int precision) {
    std::ostringstream oss;

    // Preserve non-zero decimal numbers even when precision is zero
    if(precision == 0 && std::fmod(number, 1.0) != 0.0) {
        precision = 2;
    }

    oss << std::fixed << std::setprecision(precision) << number;
    std::string s = oss.str();

    return s;
}
