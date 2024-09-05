#include <iostream>
#include <bitset>
#include <algorithm>
#include <ranges>

#include "adt.cpp"
#include "stack.h"
#include "num_utils.h"

std::optional<std::string> Stack::exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, __attribute__((unused)) std::unordered_map<char, dc::Register> &regs) {
    std::optional<std::string> err = std::nullopt;
    
    auto print_oradix = [&stack, &parameters, this](dc::radix_base base) {
        auto old_rdx = parameters.oradix;
        parameters.oradix = base;
        auto res = fn_print(stack, parameters, StackOP::P);
        parameters.oradix = old_rdx;

        return res;
    };

    switch(this->op_type) {
        case OPType::PCG: err = fn_print(stack, parameters, StackOP::PNL); break;
        case OPType::P: err = fn_print(stack, parameters, StackOP::P); break;
        case OPType::PWS: err = fn_print(stack, parameters, StackOP::PS); break;
        case OPType::PBB: err = print_oradix(dc::radix_base::BIN); break;
        case OPType::PBO: err = print_oradix(dc::radix_base::OCT); break;
        case OPType::PBH: err = print_oradix(dc::radix_base::HEX); break;
        case OPType::CLR: stack.clear(); break;
        case OPType::PH: err = fn_pop_head(stack); break;
        case OPType::SO: err = fn_swap_xy(stack); break;
        case OPType::DP: err = fn_dup_head(stack); break;
        case OPType::PS: err = fn_print_stack(stack, parameters); break;
        case OPType::CH: err = fn_head_size(stack); break;
        case OPType::CS: err = fn_stack_size(stack); break;
        case OPType::SP: err = fn_set_precision(stack, parameters); break;
        case OPType::GP: err = fn_get_precision(stack, parameters); break;
        case OPType::SOR: err = fn_set_oradix(stack, parameters); break;
        case OPType::GOR: err = fn_get_oradix(stack, parameters); break;
        case OPType::SIR: err = fn_set_iradix(stack, parameters); break;
        case OPType::GIR: err = fn_get_iradix(stack, parameters); break;
        case OPType::LX: err = fn_get_lastx(stack); break;
        case OPType::LY: err = fn_get_lasty(stack); break;
        case OPType::LZ: err = fn_get_lastz(stack); break;
        default: break;
    }

    return err;
}

/**
 * @brief Prints the head of the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * @param op The type of print operation(with newline, without newline and with space)
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_print(dc::Stack<std::string> &stack, dc::Parameters  &parameters, const StackOP op) {
    // Check if the stack is empty
    if(stack.empty()) {
        return "Cannot print empty stack";
    }

    // If the output radix is non-decimal, check if top of the stack is an integer
    if(static_cast<int>(parameters.oradix) != 10 && !NumericUtils::is_numeric<int>(stack.pop(false))) {
        return "This output radix requires integer values";
    }

    switch(parameters.oradix) {
        case dc::radix_base::DEC: {
            switch(op) {
                case StackOP::PNL: std::cout << stack.pop(false) << std::endl; break;
                case StackOP::P: std::cout << stack.pop(false); break;
                case StackOP::PS: std::cout << stack.pop(false) << ' '; break;
            }
            break;
        }
        case dc::radix_base::BIN: {
            std::bitset<64> head{std::stoul(stack.pop(false))};
            auto bin_value = bin_prettify(head.to_string());

            std::cout << bin_value << std::endl;
            break;
        }
        case dc::radix_base::OCT: {
            auto head = std::stol(stack.pop(false));
            std::cout << std::oct << head << 'o' << std::dec << std::endl;
            break;
        }
        case dc::radix_base::HEX: {
            auto head = std::stol(stack.pop(false));
            std::cout << std::hex << std::uppercase << head << 'h'
                      << std::dec << std::nouppercase << std::endl;
            break;
        }
        default: return "Unsupported output base";
    }

    return std::nullopt;
}

/**
 * @brief Removes top of the stack
 * 
 * Removes the head of the stack without printing it
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_pop_head(dc::Stack<std::string> &stack) {
    // Check if stack is empty
    if(stack.empty()) {
        return "'R' does not work on empty stack";
    }

    stack.copy_xyz();
    stack.pop(true);

    return std::nullopt;
}

/**
 * @brief Swaps the top two values of the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_swap_xy(dc::Stack<std::string> &stack) {
    // Check if the stack has enough elements
    if(stack.size() < 2) {
        return "'r' requires two elements";
    }

    // Swap top two elements
    auto len = stack.size()-1;
    auto x = stack[len];
    
    stack.copy_xyz();
    stack[len] = stack[len-1];
    stack[len-1] = x;

    return std::nullopt;
}

/**
 * @brief Duplicates the head of the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_dup_head(dc::Stack<std::string> &stack) {
    // Check if the stack has enough elements
    if(stack.empty()) {
        return "'d' requires one element";
    }

    auto head = stack.pop(false);
    stack.push(head);

    return std::nullopt;
}

/**
 * @brief Prints the entire contents of the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_print_stack(const dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    const auto& const_ref = stack.get_ref();

    switch(parameters.oradix) {
        case dc::radix_base::DEC: {
            for(const auto& it : std::ranges::reverse_view(const_ref)) {
                std::cout << it << std::endl;
            }
            break;
        }
        case dc::radix_base::BIN: {
            for(const auto& it : std::ranges::reverse_view(const_ref)) {
                std::bitset<64> head{std::stoul(it)};
                auto bin_value = bin_prettify(head.to_string());

                std::cout << bin_value << std::endl;
            }
            break;
        }
        case dc::radix_base::OCT: {
            for(const auto& it : std::ranges::reverse_view(const_ref)) {
                std::cout << std::oct << std::stol(it) << 'o' << std::dec << std::endl;
            }
            break;
        }
        case dc::radix_base::HEX: {
            for(const auto& it : std::ranges::reverse_view(const_ref)) {
                std::cout << std::hex << std::uppercase << std::stol(it) << 'h'
                          << std::dec << std::nouppercase << std::endl;
                }
            break;
        }
    }

    return std::nullopt;
}

/**
 * @brief Computes the length of the head of the stack
 * 
 * Takes one value off the stack and computes the number of 
 * digits it has(or the number of characters, if it is a string)
 * and pushes that number
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_head_size(dc::Stack<std::string> &stack) {
    // Check if the stack has enough elements
    if(stack.empty()) {
        return "'Z' does not work on empty stack";
    }

    // Take head of the stack
    auto head = stack.pop(false);

    // If it's an integer, count its digits
    if(NumericUtils::is_numeric<int>(head)) {
        auto num = std::stoi(head);

        stack.copy_xyz();
        stack.pop(true);

        std::size_t len = 0;
        while(num > 0) {
            num /= 10;
            len++;
        }

        stack.push(std::to_string(len));
    } else {
        // Otherwise, treat the value as a string and count its length
        stack.copy_xyz();
        stack.pop(true);
        head.erase(std::remove(head.begin(), head.end(), '.'), head.end());
        stack.push(std::to_string(head.length()));
    }

    return std::nullopt;
}

/**
 * @brief Calculates the size of the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_stack_size(dc::Stack<std::string> &stack) {
    stack.push(std::to_string(stack.size()));

    return std::nullopt;
}

/**
 * @brief Sets precision for numeric function
 * 
 * Takes one value from the stack and uses it to set the precision
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_set_precision(dc::Stack<std::string> &stack, dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'k' requires one operand";
    }

    // Check whether head is a non-negative number
    auto head = stack.pop(false);
    if(!NumericUtils::is_numeric<int>(head) || std::stoi(head) < 0) {
        return "Precision must be a non-negative number";
    }

    // Otherwise extract head of the stack and use it
    // to set precision parameter
    stack.copy_xyz();
    stack.pop(true);
    parameters.precision = std::stoi(head);

    return std::nullopt;
}

/**
 * @brief Gets the precision
 * 
 * Pushes the current precision parameter onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_get_precision(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    stack.push(std::to_string(parameters.precision));

    return std::nullopt;
}

/**
 * @brief Sets output radix
 * 
 * Takes one value from the stack and uses it to set the output radix
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_set_oradix(dc::Stack<std::string> &stack, dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'o' requires one operand";
    }

    // Check whether the head is a number
    stack.copy_xyz();
    auto head = stack.pop(true);
    if(!NumericUtils::is_numeric<int>(head)) {
        return "'o' requires numeric values only";
    }

    // Otherwise convert it to int
    auto oradix = std::stoi(head);
    switch(oradix) {
        case 2: parameters.oradix = dc::radix_base::BIN; break;
        case 8: parameters.oradix = dc::radix_base::OCT; break;
        case 10: parameters.oradix = dc::radix_base::DEC; break;
        case 16: parameters.oradix = dc::radix_base::HEX; break;
        default: return "'o' accepts either BIN, OCT, DEC or HEX bases";
    }

    return std::nullopt;
}

/**
 * @brief Gets output radix
 * 
 * Pushes the current output radix parameter onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_get_oradix(dc::Stack<std::string> &stack, dc::Parameters &parameters) {
    stack.push(std::to_string(static_cast<int>(parameters.oradix)));

    return std::nullopt;
}

/**
 * @brief Sets input radix
 * 
 * Takes one value from the stack and uses it to set the input radix
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_set_iradix(dc::Stack<std::string> &stack, dc::Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'i' requires one operand";
    }

    // Check whether head is a number within the range 2-16
    auto head = stack.pop(false);
    if(!NumericUtils::is_numeric<double>(head) || std::stoi(head) < 2 || std::stoi(head) > 16) {
        return "Input base must be a number within the range 2-16(inclusive)";
    }

    // Otherwise extract head of the stack and use it
    // to set input base
    stack.copy_xyz();
    stack.pop(true);
    parameters.iradix = std::stoi(head);

    return std::nullopt;
}

/**
 * @brief Gets input radix
 * 
 * Pushes the current input radix parameter onto the stack
 * 
 * @param stack An instance of the dc::Stack data structure
 * @param parameters An instance of the dc::Parameters data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_get_iradix(dc::Stack<std::string> &stack, const dc::Parameters &parameters) {
    stack.push(std::to_string(parameters.iradix));

    return std::nullopt;
}

/**
 * @brief Gets last value of top of the stack
 * 
 * Retrieves the values that was last top-of-stack before execution of any operation.
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_get_lastx(dc::Stack<std::string> &stack) {
    // Retrieve last x from the stack and push it back
    auto last_x = stack.get_last_x();
    last_x.empty() ? stack.push("0") : stack.push(last_x);

    return std::nullopt;
}

/**
 * @brief Gets last value of second to top of the stack
 * 
 * Retrieves the values that was last second-of-stack before execution of any operation.
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_get_lasty(dc::Stack<std::string> &stack) {
    // Retrieve last y from the stack and push it back
    auto last_y = stack.get_last_y();
    last_y.empty() ? stack.push("0") : stack.push(last_y);

    return std::nullopt;
}

/**
 * @brief Gets last value of third to top of the stack
 * 
 * Retrieves the values that was last third-of-stack before execution of any operation.
 * 
 * @param stack An instance of the dc::Stack data structure
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Stack::fn_get_lastz(dc::Stack<std::string> &stack) {
    // Retrieve last y from the stack and push it back
    auto last_z = stack.get_last_z();
    last_z.empty() ? stack.push("0") : stack.push(last_z);

    return std::nullopt;
}

/**
 * @brief Pretty prints a binary number
 * 
 * Formats a binary number by removing insignificant digits
 * and by appending a 'b' character at the end of the number
 * 
 * @param s A string representing the binary number
 * 
 * @return A string containing the formatted binary number
 */
std::string Stack::bin_prettify(std::string s) {
    std::string result;

    // Find the first '1' on the binary string
    auto pos = s.find('1');

    if(pos != std::string::npos) {
        // Take only the significant part of the number
        result = s.substr(pos);
    } else {
        // Otherwise the number is 'zero'
        result = '0';
    }

    // Add 'b' suffix
    result.push_back('b');

    return result;
}
