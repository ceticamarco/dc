#include <cmath>
#include <numbers>
#include <iomanip>

#include "adt.cpp"
#include "math.h"
#include "is_num.h"

std::optional<std::string> Math::exec(DCStack<std::string> &stack, Parameters &parameters, __attribute__((unused))  std::unordered_map<char, Register> &regs) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        case OPType::ADD: err = fn_add(stack, parameters); break;
        case OPType::SUB: err = fn_sub(stack, parameters); break;
        case OPType::MUL: err = fn_mul(stack, parameters); break;
        case OPType::DIV: err = fn_div(stack, parameters); break;
        case OPType::MOD: err = fn_mod(stack, parameters); break;
        case OPType::DIV_MOD: err = fn_div_mod(stack, parameters); break;
        case OPType::MOD_EXP: err = fn_mod_exp(stack, parameters); break;
        case OPType::EXP: err = fn_exp(stack, parameters); break;
        case OPType::SQRT: err = fn_sqrt(stack, parameters); break;
        case OPType::SIN: err = fn_sin(stack, parameters); break;
        case OPType::COS: err = fn_cos(stack, parameters); break;
        case OPType::TAN: err = fn_tan(stack, parameters); break;
        case OPType::ASIN: err = fn_asin(stack, parameters); break;
        case OPType::ACOS: err = fn_acos(stack, parameters); break;
        case OPType::ATAN: err = fn_atan(stack, parameters); break;
        case OPType::FACT: err = fn_fact(stack, parameters); break;
        case OPType::PI: err = fn_pi(stack, parameters); break;
        case OPType::E: err = fn_e(stack, parameters); break;
        default: break;
    }

    return err;
}

std::optional<std::string> Math::fn_add(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto lhs = std::stod(stack.pop(true));
        auto rhs = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits((lhs + rhs), parameters.precision));
    } else {
        return "'+' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sub(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto lhs = std::stod(stack.pop(true));
        auto rhs = std::stod(stack.pop(true));

        // Subtract the two operands
        auto result = (-(lhs - rhs));

        // Compare the result with an epsilon value
        // to prevent -0/+0 results
        auto epsilon = 1e-10;
        if(std::abs(result) < epsilon) {
            result = 0.0;
        }

        // Push back the result as a string
        stack.push(trim_digits(result, parameters.precision));
    } else {
        return "'-' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mul(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto lhs = std::stod(stack.pop(true));
        auto rhs = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits((lhs * rhs), parameters.precision));
    } else {
        return "'*' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_div(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto divisor = std::stod(stack.pop(true));
        auto dividend = std::stod(stack.pop(true));

        // Check whether divisor is equal to zero
        if(divisor == 0.0) {
            return "Cannot divide by zero";
        }

        // Push back the result as a string
        stack.push(trim_digits((dividend / divisor), parameters.precision));
    } else {
        return "'/' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mod(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto rhs = std::stod(stack.pop(true));
        auto lhs = std::stod(stack.pop(true));

        // Check whether divisor is equal to zero
        if(rhs == 0.0) {
            return "Cannot divide by zero";
        }

        // Push back the result as a string
        stack.push(trim_digits((static_cast<int>(lhs) % static_cast<int>(rhs)), parameters.precision));
    } else {
        return "'%' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_div_mod(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto divisor = std::stod(stack.pop(true));
        auto dividend = std::stod(stack.pop(true));

        // Check if divisor is not equal to zero
        if(divisor != 0.0) {
            auto quotient = std::trunc(dividend / divisor);
            auto remainder = ((int)dividend % (int)divisor);

            stack.push(trim_digits(quotient, parameters.precision));
            stack.push(trim_digits(remainder, parameters.precision));
        }

    } else {
        return "'~' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_mod_exp(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto modulus = std::stoi(stack.pop(true));
        auto exponent = std::stoi(stack.pop(true));
        auto base = std::stoi(stack.pop(true));

        if(modulus == 1) {
            stack.push("0");
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
        
        stack.push(trim_digits(c, parameters.precision));
    } else {
        return "'|' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_exp(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto exp = std::stod(stack.pop(true));
        auto base = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(pow(base, exp), parameters.precision));
    } else {
        return "'^' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sqrt(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        if(val <= 0.0) {
            return "'v' domain error";
        }

        // Push back the result as a string
        stack.push(trim_digits(sqrt(val), parameters.precision));
    } else {
        return "'v' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_sin(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(sin(val), parameters.precision));
    } else {
        return "'sin' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_cos(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(cos(val), parameters.precision));
    } else {
        return "'cos' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_tan(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(tan(val), parameters.precision));
    } else {
        return "'tan' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_asin(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(asin(val), parameters.precision));
    } else {
        return "'asin' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_acos(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(acos(val), parameters.precision));
    } else {
        return "'acos' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_atan(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(atan(val), parameters.precision));
    } else {
        return "'atan' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_fact(DCStack<std::string> &stack, Parameters &parameters) {
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
        stack.copy_xyz();
        unsigned long factorial = 1;
        auto val = std::stod(stack.pop(true));

        if(val == 0.0) {
            factorial = 1;
        }

        for(size_t i = 2; i <= (size_t)val; i++) {
            factorial *= i;
        }

        // Push back the result as a string
        stack.push(trim_digits(static_cast<double>(factorial), parameters.precision));
    } else {
        return "'!' requires numeric values";
    }

    return std::nullopt;
}

std::optional<std::string> Math::fn_pi(DCStack<std::string> &stack, Parameters &parameters) {
    stack.push(trim_digits(std::numbers::pi, parameters.precision));

    return std::nullopt;
}

std::optional<std::string> Math::fn_e(DCStack<std::string> &stack, Parameters &parameters) {
    stack.push(trim_digits(std::numbers::e, parameters.precision));

    return std::nullopt;
}

std::string Math::trim_digits(double number, unsigned int precision) {
    std::ostringstream oss;

    // Preserve non-zero decimal numbers even when precision is zero
    if(precision == 0 && std::fmod(number, 1.0) != 0.0) {
        precision = 2;
    }

    oss << std::fixed << std::setprecision(static_cast<int>(precision)) << number;
    std::string s = oss.str();

    return s;
}
