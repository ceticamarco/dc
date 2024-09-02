#include <cmath>
#include <numbers>
#include <complex>
#include <iomanip>
#include <random>

#include "adt.cpp"
#include "mathematics.h"
#include "is_num.h"

std::optional<std::string> Mathematics::exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, __attribute__((unused))  std::unordered_map<char, dc::Register> &regs) {
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
        case OPType::RND: err = fn_random(stack, parameters); break;
        case OPType::INT: err = fn_integer(stack, parameters); break;
        case OPType::TO_CMPLX: err = fn_to_complex(stack, parameters); break;
        case OPType::GET_RE: err = fn_get_real(stack, parameters); break;
        case OPType::GET_IM: err = fn_get_imaginary(stack, parameters); break;
        case OPType::LOG: err = fn_log10(stack, parameters); break;
        default: break;
    }

    return err;
}

/**
 * @brief Perform a sum
 * 
 * Take two numbers from the stack and sum them together. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_add(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'+' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);
    auto is_x_cmplx = is_complex(x);
    auto is_y_cmplx = is_complex(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        auto rhs = std::stod(stack.pop(true));
        auto lhs = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits((lhs + rhs), parameters.precision));
    } else if(is_x_cmplx || is_y_cmplx) {
        stack.copy_xyz();
        // Convert complex dc objects(ie strings) to std::complex
        std::complex<double> rhs, lhs;
        std::istringstream ss1(stack.pop(true));
        std::istringstream ss2(stack.pop(true));
        ss1 >> rhs, ss2 >> lhs;

        std::complex<double> sum = (rhs + lhs);

        // trim their digits
        auto real_trimmed = trim_digits(sum.real(), parameters.precision);
        auto imag_trimmed = trim_digits(sum.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'+' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Perform a subtraction
 * 
 * Take two numbers from the stack and subtract the first one popped from the second one popped. 
 * Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_sub(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'-' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);
    auto is_x_cmplx = is_complex(x);
    auto is_y_cmplx = is_complex(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        auto rhs = std::stod(stack.pop(true));
        auto lhs = std::stod(stack.pop(true));

        // Subtract the two operands
        auto result = (lhs - rhs);

        // Compare the result with an epsilon value
        // to prevent -0/+0 results
        auto epsilon = 1e-10;
        if(std::abs(result) < epsilon) {
            result = 0.0;
        }

        // Push back the result as a string
        stack.push(trim_digits(result, parameters.precision));
    } else if(is_x_cmplx || is_y_cmplx) {
        stack.copy_xyz();
        // Convert complex dc objects(ie strings) to std::complex
        std::complex<double> rhs, lhs;
        std::istringstream ss1(stack.pop(true));
        std::istringstream ss2(stack.pop(true));
        ss1 >> rhs, ss2 >> lhs;

        std::complex<double> diff = (lhs - rhs);

        // trim their digits
        auto real_trimmed = trim_digits(diff.real(), parameters.precision);
        auto imag_trimmed = trim_digits(diff.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'-' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Perform a multiplication
 * 
 * Take two numbers from the stack and multiply them together. 
 * Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_mul(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'*' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);
    auto is_x_cmplx = is_complex(x);
    auto is_y_cmplx = is_complex(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        auto rhs = std::stod(stack.pop(true));
        auto lhs = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits((lhs * rhs), parameters.precision));
    } else if(is_x_cmplx || is_y_cmplx) {
        stack.copy_xyz();
        // Convert complex dc objects(ie strings) to std::complex
        std::complex<double> rhs, lhs;
        std::istringstream ss1(stack.pop(true));
        std::istringstream ss2(stack.pop(true));
        ss1 >> rhs, ss2 >> lhs;

        std::complex<double> mul = (lhs * rhs);

        // trim their digits
        auto real_trimmed = trim_digits(mul.real(), parameters.precision);
        auto imag_trimmed = trim_digits(mul.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'*' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Perform a division
 * 
 * Take two numbers from the stack and divide the second one popped from the first one popped.
 * Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_div(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'/' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);
    auto is_x_cmplx = is_complex(x);
    auto is_y_cmplx = is_complex(y);

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
    } else if(is_x_cmplx || is_y_cmplx) {
        stack.copy_xyz();
        // Convert complex dc objects(ie strings) to std::complex
        std::complex<double> divisor, dividend;
        std::istringstream ss1(stack.pop(true));
        std::istringstream ss2(stack.pop(true));
        ss1 >> divisor, ss2 >> dividend;

        // Check whether divisor is equal to zero
        if(divisor == 0.0) {
            return "Cannot divide by zero";
        }

        std::complex<double> div = (dividend / divisor);

        // trim their digits
        auto real_trimmed = trim_digits(div.real(), parameters.precision);
        auto imag_trimmed = trim_digits(div.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'/' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Computes the modulo
 * 
 * Take two values from the stack and compute the remainder of the division between the second one popped
 * and the first one popped. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_mod(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'%' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
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

/**
 * @brief Compute the quotient and the remainder
 * 
 * Take two values from the stack and divide the second one popped from the first one popped. The quotient is pushed first,
 * and then the remainder.
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_div_mod(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'~' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
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

/**
 * @brief Compute the modular exponentiation
 * 
 * Take three values from the stack and compute their modular exponentiation using the first value popped as the reduction
 * modulus, the second one popped as the exponent and the third one popped as the base to be exponentiated. 
 * This method computes the following modular equivalence: c ≡ b^e (mod n)
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_mod_exp(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 3) {
        return "'|' requires three operands";
    }

    // Otherwise extract three elements from the stack.
	// The first one is the modulus(n), the second one
	// is the exponent(e) and the third one is the base(b)
    auto len = stack.size()-1;
    auto n = stack[len];
    auto e = stack[len-1];
    auto b = stack[len-2];
    auto is_n_num = is_num<long long>(n);
    auto is_e_num = is_num<long long>(e);
    auto is_b_num = is_num<long long>(b);

    // This functions computes
	// 		c ≡ b^e (mod n)
    if(is_n_num && is_e_num && is_b_num) {
        stack.copy_xyz();
        auto modulus = std::stoll(stack.pop(true));
        auto exponent = std::stoll(stack.pop(true));
        auto base = std::stoll(stack.pop(true));

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

/**
 * @brief Compute the exponentiation
 * 
 * Take two values from the stack and compute their exponentiates, using the first value popped as the
 * exponent and the second popped as the base
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_exp(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'^' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto y = stack[len-1];
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);
    auto is_x_cmplx = is_complex(x);
    auto is_y_cmplx = is_complex(y);

    // Check whether both entries are numbers
    if(is_x_num && is_y_num) {
        stack.copy_xyz();
        auto exp = std::stod(stack.pop(true));
        auto base = std::stod(stack.pop(true));

        std::complex<double> power;

        // If base is positive or exponent is an integer
        // calculate pow with real numbers. Otherwise
        // with complex numbers
        if(base > 0 || std::fmod(exp, 1.0) == 0) {
            power = std::pow(base, exp);
        } else {
            power = std::pow(std::complex<double>(base), exp);
        }

        // Check if result is a complex number
        if(std::imag(power) != 0) {
            // trim their digits
            auto real_trimmed = trim_digits(power.real(), parameters.precision);
            auto imag_trimmed = trim_digits(power.imag(), parameters.precision);
            auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

            // Push the result back onto the stack
            stack.push(complex_str);
        } else {
            // Push the result back onto the stack
            stack.push(trim_digits(std::real(power), parameters.precision));
        }
    } else if(is_x_cmplx || is_y_cmplx) {
        stack.copy_xyz();
        // Convert complex dc objects(ie strings) to std::complex
        std::complex<double> exp, base;
        std::istringstream ss1(stack.pop(true));
        std::istringstream ss2(stack.pop(true));
        ss1 >> exp, ss2 >> base;

        std::complex<double> power = std::pow(base, exp);

        // trim their digits
        auto real_trimmed = trim_digits(power.real(), parameters.precision);
        auto imag_trimmed = trim_digits(power.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'^' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the square root
 * 
 * Take one value from the stack and compute its square root. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_sqrt(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'v' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_y_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num || is_y_cmplx) {
        stack.copy_xyz();
        auto val = stack.pop(true);

        if(is_complex(val) || std::stod(val) < 0) {
            std::complex<double> c_val;
            std::istringstream ss1(val);
            ss1 >> c_val;

            std::complex<double> sq = std::sqrt(c_val);

            // trim their digits
            auto real_trimmed = trim_digits(sq.real(), parameters.precision);
            auto imag_trimmed = trim_digits(sq.imag(), parameters.precision);
            auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

            // Push the result back onto the stack
            stack.push(complex_str);
        } else {
            stack.push(trim_digits(sqrt(std::stod(val)), parameters.precision));
        }
        
    } else {
        return "'v' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the sine
 * 
 * Take one value from the stack and compute the sine function. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_sin(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'sin' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_x_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(sin(val), parameters.precision));
    } else if(is_x_cmplx) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> s = sin(c_val);
        
        // trim their digits
        auto real_trimmed = trim_digits(s.real(), parameters.precision);
        auto imag_trimmed = trim_digits(s.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'sin' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the cosine
 * 
 * Take one value from the stack and compute the cosine. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_cos(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'cos' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_x_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(cos(val), parameters.precision));
    } else if(is_x_cmplx) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> s = cos(c_val);
        
        // trim their digits
        auto real_trimmed = trim_digits(s.real(), parameters.precision);
        auto imag_trimmed = trim_digits(s.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'cos' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the tangent
 * 
 * Take one value from the stack and compute the tangent. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_tan(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'tan' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_x_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(tan(val), parameters.precision));
    } else if(is_x_cmplx) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> s = tan(c_val);
        
        // trim their digits
        auto real_trimmed = trim_digits(s.real(), parameters.precision);
        auto imag_trimmed = trim_digits(s.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'tan' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the arcsine
 * 
 * Take one value from the stack and compute the arcsine. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_asin(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'asin' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_x_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(asin(val), parameters.precision));
    } else if(is_x_cmplx) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> s = asin(c_val);
        
        // trim their digits
        auto real_trimmed = trim_digits(s.real(), parameters.precision);
        auto imag_trimmed = trim_digits(s.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'asin' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the arcosine
 * 
 * Take one value from the stack and compute the arcosine. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_acos(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'acos' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_x_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(acos(val), parameters.precision));
    } else if(is_x_cmplx) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> s = acos(c_val);
        
        // trim their digits
        auto real_trimmed = trim_digits(s.real(), parameters.precision);
        auto imag_trimmed = trim_digits(s.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'acos' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the arctangent
 * 
 * Take one value from the stack and compute the arctangent. Push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_atan(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'atan' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);
    auto is_x_cmplx = is_complex(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(atan(val), parameters.precision));
    } else if(is_x_cmplx) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> s = atan(c_val);
        
        // trim their digits
        auto real_trimmed = trim_digits(s.real(), parameters.precision);
        auto imag_trimmed = trim_digits(s.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'atan' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Compute the factorial
 * 
 * Take one value from the stack and compute the factorial. Pushe the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_fact(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'!' requires one operand";
    }

    // Extract one entry from the stack
    auto len = stack.size()-1;
    auto x = stack[len];
    auto is_x_num = is_num<double>(x);

    // Check whether the entry is a number
    if(is_x_num) {
        stack.copy_xyz();
        unsigned long long factorial = 1;
        auto val = std::stod(stack.pop(true));

        if(val < 0.0) {
            return "'!' is not defined for negative numbers";
        }

        for(unsigned long long i = 2; i <= val; i++) {
            factorial *= i;
        }

        // Push back the result as a string
        stack.push(trim_digits(static_cast<double>(factorial), parameters.precision));
    } else {
        return "'!' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Push the PI constant
 * 
 * Push the PI constant onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 */
std::optional<std::string> Mathematics::fn_pi(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    stack.push(trim_digits(std::numbers::pi, parameters.precision));

    return std::nullopt;
}

/**
 * @brief Push the _e_ constant
 * 
 * Push the _e_ constant onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 */
std::optional<std::string> Mathematics::fn_e(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    stack.push(trim_digits(std::numbers::e, parameters.precision));

    return std::nullopt;
}

/**
 * @brief Generate a random number
 * 
 * Take two values from the stack and convert and generate a random number using the first value popped as the upper bound and the second one popped
 * as the lower bound.
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_random(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'@' requires two operands";
    }

    // Extract two entries from the stack
    auto len = stack.size() - 1;
    auto b = stack[len];
    auto a = stack[len-1];
    auto is_a_num = is_num<double>(a);
    auto is_b_num = is_num<double>(b);

    // Check whether both entries are numbers
    if(is_a_num && is_b_num) {
        stack.copy_xyz();
        auto u_bound = std::stod(stack.pop(true));
        auto l_bound = std::stod(stack.pop(true));
        
        // Initialize random distribution with user bounds( [a, b] )
        std::random_device r_dev;
        std::mt19937_64 rng(r_dev());
        std::uniform_real_distribution<double> u_dist(l_bound, u_bound);
        auto r_number = u_dist(rng);

        // Push the random value onto the stack
        stack.push(trim_digits(r_number, parameters.precision));
    } else {
        return "'@' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Convert a number to the nearest integer of lesser magnitude
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_integer(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'$' requires one operand";
    }

    auto head = stack.pop(false);
    auto is_head_num = is_num<double>(head);
    
    // Check whether head of the stack is a number
    if(is_head_num) {
        stack.copy_xyz();
        // Convert to integral type to truncate
        auto value = std::stol(stack.pop(true));
        // Push the truncated number back to the stack
        stack.push(trim_digits(static_cast<double>(value), parameters.precision));
    } else {
        return "'$' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Insert a complex number to the stack
 * 
 * Take two number from the stack and use them to create a new complex number of the form a + ib, where _a_
 * is the second-to-top of the stack and _b_ is the head of the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_to_complex(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.size() < 2) {
        return "'b' requires two values";
    }

    auto len = stack.size()-1;
    auto x = stack.at(len);
    auto y = stack.at(len-1);
    auto is_x_num = is_num<double>(x);
    auto is_y_num = is_num<double>(y);

    // Check whether both values are numbers
    if(is_x_num && is_y_num) {
        // Extract the real and imaginary part of the complex number
        auto imag = std::stod(stack.pop(true));
        auto real = std::stod(stack.pop(true));

        // trim their digits
        auto real_trimmed = trim_digits(real, parameters.precision);
        auto imag_trimmed = trim_digits(imag, parameters.precision);

        // Complex numbers are represented on the stack as "(Re,Im)"
        // They are then converted to std::complex before computations
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');
        stack.push(complex_str);
    } else {
        return "'b' requires numeric values";
    }

    return std::nullopt;
}


std::string Mathematics::trim_digits(double number, unsigned int precision) {
    std::ostringstream oss;

    // Preserve non-zero decimal numbers even when precision is zero
    if(precision == 0 && std::fmod(number, 1.0) != 0.0) {
        precision = 2;
    }

    oss << std::fixed << std::setprecision(static_cast<int>(precision)) << number;
    std::string s = oss.str();

    return s;
}

/**
 * @brief Extract the real part of a complex number
 * 
 * Take one complex number from the stack and extract its real part
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_get_real(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'re' requires one value";
    }

    auto head = stack.pop(false);
    auto is_head_complex = is_complex(head);

    if(is_head_complex) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        // Get real part
        auto real = c_val.real();
    
        // Push the result back onto the stack
        stack.push(trim_digits(real, parameters.precision));
    } else {
        return "'re' requires complex values";
    }

    return std::nullopt;
}

/**
 * @brief Extract the imaginary part of a complex number
 * 
 * Take one complex number from the stack and extract its imaginary part
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_get_imaginary(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'im' requires one value";
    }

    auto head = stack.pop(false);
    auto is_head_complex = is_complex(head);

    if(is_head_complex) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        // Get imaginary part
        auto imag = c_val.imag();
    
        // Push the result back onto the stack
        stack.push(trim_digits(imag, parameters.precision));
    } else {
        return "'im' requires complex values";
    }

    return std::nullopt;
}

/**
 * @brief Calculate the common logarithm
 * 
 * Take one value from the stack and compute its common(base 10) logarith, then 
 * push the result back onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Paramaters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Mathematics::fn_log10(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'y' requires one value";
    }

    auto head = stack.pop(false);
    auto is_head_num = is_num<double>(head);
    auto is_head_complex = is_complex(head);

    if(is_head_num) {
        stack.copy_xyz();
        auto val = std::stod(stack.pop(true));

        // Push back the result as a string
        stack.push(trim_digits(log10(val), parameters.precision));
    } else if(is_head_complex) {
        stack.copy_xyz();
        std::complex<double> c_val;
        std::istringstream ss1(stack.pop(true));
        ss1 >> c_val;

        std::complex<double> lg = log(c_val);

        // trim their digits
        auto real_trimmed = trim_digits(lg.real(), parameters.precision);
        auto imag_trimmed = trim_digits(lg.imag(), parameters.precision);
        auto complex_str = ('(' + real_trimmed + ',' + imag_trimmed + ')');

        // Push the result back onto the stack
        stack.push(complex_str);
    } else {
        return "'y' requires numeric values";
    }

    return std::nullopt;
}

/**
 * @brief Return true if **str** is a complex number, false otherwise
 * 
 * @param str A string containing a number
 * 
 * @return boolean value
 */
bool Mathematics::is_complex(const std::string &str) {
    // Complex numbers are represented on the stack as "(Re,Im)"
    if(str.front() != '(' || str.back() != ')') {
        return false;
    }

    // Extract "Re,Im" without parenthesis
    std::istringstream ss(str.substr(1, str.size() - 2));
    double real, imag;
    char comma;

    return (ss >> real >> comma >> imag) && ss.eof();
}