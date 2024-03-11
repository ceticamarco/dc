#include <iostream>
#include <algorithm>
#include <ranges>

#include "adt.cpp"
#include "stack.h"
#include "is_num.h"

std::optional<std::string> Stack::exec(DCStack<std::string> &stack, Parameters &parameters, __attribute__((unused)) std::unordered_map<char, Register> &regs) {
    std::optional<std::string> err = std::nullopt;
    
    auto print_oradix = [&stack, &parameters, this](radix_base base) {
        auto old_rdx = parameters.oradix;
        parameters.oradix = base;
        auto res = fn_print(stack, parameters, false);
        parameters.oradix = old_rdx;

        return res;
    };

    switch(this->op_type) {
        case OPType::PCG: err = fn_print(stack, parameters, true); break;
        case OPType::P: err = fn_print(stack, parameters, false); break;
        case OPType::PBB: err = print_oradix(radix_base::BIN); break;
        case OPType::PBO: err = print_oradix(radix_base::OCT); break;
        case OPType::PBH: err = print_oradix(radix_base::HEX); break;
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
        default: break;
    }

    return err;
}

std::optional<std::string> Stack::fn_print(DCStack<std::string> &stack, Parameters  &parameters, bool new_line) {
    // Check if the stack is empty
    if(stack.empty()) {
        return "Cannot print empty stack";
    }

    // If the output radix is non-decimal, check if top of the stack is an integer
    if(static_cast<int>(parameters.oradix) != 10 && !is_num<int>(stack.pop(false))) {
        return "This output radix requires integer values";
    }

    switch(parameters.oradix) {
        case radix_base::DEC: {
            if(new_line) {
                std::cout << stack.pop(false) << std::endl;
            } else {
                std::cout << stack.pop(false);
            }
            break;
        }
        case radix_base::BIN: {
            auto head = std::stol(stack.pop(false));
            std::cout << to_bin(head) << 'b' << std::endl;
            break;
        }
        case radix_base::OCT: {
            auto head = std::stol(stack.pop(false));
            std::cout << std::oct << head << 'o' << std::dec << std::endl;
            break;
        }
        case radix_base::HEX: {
            auto head = std::stol(stack.pop(false));
            std::cout << std::hex << std::uppercase << head << 'h'
                      << std::dec << std::nouppercase << std::endl;
            break;
        }
        default: return "Unsupported output base";
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_pop_head(DCStack<std::string> &stack) {
    // Check if stack is empty
    if(stack.empty()) {
        return "'R' does not work on empty stack";
    }

    stack.pop(true);

    return std::nullopt;
}

std::optional<std::string> Stack::fn_swap_xy(DCStack<std::string> &stack) {
    // Check if the stack has enough elements
    if(stack.size() < 2) {
        return "'r' requires two elements";
    }

    // Swap top two elements
    auto len = stack.size()-1;
    auto x = stack.at(len);
    
    stack.at(len) = stack.at(len-1);
    stack.at(len-1) = x;

    return std::nullopt;
}

std::optional<std::string> Stack::fn_dup_head(DCStack<std::string> &stack) {
    // Check if the stack has enough elements
    if(stack.empty()) {
        return "'d' requires one element";
    }

    auto head = stack.pop(false);
    stack.push(head);

    return std::nullopt;
}

std::optional<std::string> Stack::fn_print_stack(DCStack<std::string> &stack, Parameters &parameters) {
    const auto& const_ref = stack.get_ref();

    switch(parameters.oradix) {
        case radix_base::DEC: {
            for(auto & it : std::ranges::reverse_view(const_ref)) {
                std::cout << it << std::endl;
            }
            break;
        }
        case radix_base::BIN: {
            for(auto & it : std::ranges::reverse_view(const_ref)) {
                std::cout << to_bin(std::stol(it)) << 'b' << std::endl;
            }
            break;
        }
        case radix_base::OCT: {
            for(auto & it : std::ranges::reverse_view(const_ref)) {
                std::cout << std::oct << std::stol(it) << 'o' << std::dec << std::endl;
            }
            break;
        }
        case radix_base::HEX: {
            for(auto & it : std::ranges::reverse_view(const_ref)) {
                std::cout << std::hex << std::uppercase << std::stol(it) << 'h'
                          << std::dec << std::nouppercase << std::endl;
                }
            break;
        }
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_head_size(DCStack<std::string> &stack) {
    // Check if the stack has enough elements
    if(stack.empty()) {
        return "'Z' does not work on empty stack";
    }

    // Take head of the stack
    auto head = stack.pop(false);

    // If it's an integer, count its digits
    if(is_num<int>(head)) {
        auto num = std::stoi(head);
        stack.pop(true);

        size_t len = 0;
        while(num > 0) {
            num /= 10;
            len++;
        }

        stack.push(std::to_string(len));
    } else {
        // Otherwise, treat the value as a string and count its length
        stack.pop(true);
        head.erase(std::remove(head.begin(), head.end(), '.'), head.end());
        stack.push(std::to_string(head.length()));
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_stack_size(DCStack<std::string> &stack) {
    stack.push(std::to_string(stack.size()));

    return std::nullopt;
}

std::optional<std::string> Stack::fn_set_precision(DCStack<std::string> &stack, Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'k' requires one operand";
    }

    // Check whether head is a non-negative number
    auto head = stack.pop(false);
    if(!is_num<int>(head) || std::stoi(head) < 0) {
        return "Precision must be a non-negative number";
    }

    // Otherwise extract head of the stack and use it
    // to set precision parameter
    stack.pop(true);
    parameters.precision = std::stoi(head);

    return std::nullopt;
}

std::optional<std::string> Stack::fn_get_precision(DCStack<std::string> &stack, Parameters &parameters) {
    stack.push(std::to_string(parameters.precision));

    return std::nullopt;
}

std::optional<std::string> Stack::fn_set_oradix(DCStack<std::string> &stack, Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'o' requires one operand";
    }

    // Check whether the head is a number
    auto head = stack.pop(false);
    if(!is_num<int>(head)) {
        return "'o' requires numeric values only";
    }

    // Otherwise convert it to int
    auto oradix = std::stoi(head);
    switch(oradix) {
        case 2: parameters.oradix = radix_base::BIN; break;
        case 8: parameters.oradix = radix_base::OCT; break;
        case 10: parameters.oradix = radix_base::DEC; break;
        case 16: parameters.oradix = radix_base::HEX; break;
        default: return "'o' accepts either BIN, OCT, DEC or HEX bases";
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_get_oradix(DCStack<std::string> &stack, Parameters &parameters) {
    stack.push(std::to_string(static_cast<int>(parameters.oradix)));

    return std::nullopt;
}

std::optional<std::string> Stack::fn_set_iradix(DCStack<std::string> &stack, Parameters &parameters) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "'i' requires one operand";
    }

    // Check whether head is a number within the range 2-16
    auto head = stack.pop(false);
    if(!is_num<double>(head) || std::stoi(head) < 2 || std::stoi(head) > 16) {
        return "Input base must be a number within the range 2-16(inclusive)";
    }

    // Otherwise extract head of the stack and use it
    // to set input base
    stack.pop(true);
    parameters.iradix = std::stoi(head);

    return std::nullopt;
}

std::optional<std::string> Stack::fn_get_iradix(DCStack<std::string> &stack, Parameters &parameters) {
    stack.push(std::to_string(parameters.iradix));

    return std::nullopt;
}

constexpr std::string Stack::to_bin(auto num) {
    if(num == 0) {
        return "0";
    }

    std::string res;
    while(num > 0) {
        res = (std::to_string(num % 2) + res);
        num /= 2;
    }

    // Remove extra zeros
    auto first_one = res.find('1');
    if(first_one != std::string::npos) {
        res = res.substr(first_one);
    }

    return res;
}
