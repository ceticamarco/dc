#include <iostream>
#include <algorithm>
#include <ranges>

#include "stack.h"
#include "is_num.h"

std::optional<std::string> Stack::exec(dc_stack_t &stack) {
    std::optional<std::string> err = std::nullopt;
    
    auto print_oradix = [this, &stack](radix_base base) {
        auto old_rdx = this->oradix;
        this->oradix = base;
        auto res = fn_print(stack, false);
        this->oradix = old_rdx;

        return res;
    };

    switch(this->op_type) {
        case OPType::PCG: err = fn_print(stack, true); break;
        case OPType::P: err = fn_print(stack, false); break;
        case OPType::PBB: err = print_oradix(radix_base::BIN); break;
        case OPType::PBO: err = print_oradix(radix_base::OCT); break;
        case OPType::PBH: err = print_oradix(radix_base::HEX); break;
        case OPType::CLR: stack.clear(); break;
        case OPType::PH: fn_pop_head(stack); break;
        case OPType::SO: fn_swap_xy(stack); break;
        case OPType::DP: fn_dup_head(stack); break;
        case OPType::PS: fn_print_stack(stack); break;
        case OPType::CH: fn_head_size(stack); break;
        case OPType::CS: fn_stack_size(stack); break;
        default: break;
    }

    return err;
}

std::optional<std::string> Stack::fn_print(dc_stack_t &stack, bool new_line) {
    // Check if the stack is empty
    if(stack.empty()) {
        return "Cannot print empty stack";
    }

    // If the output radix is non-decimal, check if top of the stack is an integer
    if(static_cast<int>(this->oradix) != 10 && !is_num<int>(stack.back())) {
        return "This output radix requires integer values";
    }

    switch(this->oradix) {
        case radix_base::DEC: {
            if(new_line) {
                std::cout << stack.back() << std::endl;
            } else {
                std::cout << stack.back();
            }
            break;
        }
        case radix_base::BIN: {
            auto head = std::stol(stack.back());
            std::cout << to_bin(head) << 'b' << std::endl;
            break;
        }
        case radix_base::OCT: {
            auto head = std::stol(stack.back());
            std::cout << std::oct << head << 'o' << std::dec << std::endl;
            break;
        }
        case radix_base::HEX: {
            auto head = std::stol(stack.back());
            std::cout << std::hex << std::uppercase << head << 'h'
                      << std::dec << std::nouppercase << std::endl;
            break;
        }
        default: return "Unsupported output base";
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_pop_head(dc_stack_t &stack) {
    // Check if stack is empty
    if(stack.empty()) {
        return "'R' does not work on empty stack";
    }

    stack.pop_back();

    return std::nullopt;
}

std::optional<std::string> Stack::fn_swap_xy(dc_stack_t &stack) {
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

std::optional<std::string> Stack::fn_dup_head(dc_stack_t &stack) {
    // Check if the stack has enough elements
    if(stack.empty()) {
        return "'d' requires one element";
    }

    auto head = stack.back();
    stack.push_back(head);

    return std::nullopt;
}

std::optional<std::string> Stack::fn_print_stack(dc_stack_t &stack) {
    switch(this->oradix) {
        case radix_base::DEC: {
            for(auto & it : std::ranges::reverse_view(stack)) {
                std::cout << it << std::endl;
            }
            break;
        }
        case radix_base::BIN: {
            for(auto & it : std::ranges::reverse_view(stack)) {
                std::cout << to_bin(std::stol(it)) << 'b' << std::endl;
            }
            break;
        }
        case radix_base::OCT: {
            for(auto & it : std::ranges::reverse_view(stack)) {
                std::cout << std::oct << std::stol(it) << 'o' << std::dec << std::endl;
            }
            break;
        }
        case radix_base::HEX: {
            for(auto & it : std::ranges::reverse_view(stack)) {
                std::cout << std::hex << std::uppercase << std::stol(it) << 'h'
                          << std::dec << std::nouppercase << std::endl;
                }
            break;
        }
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_head_size(dc_stack_t &stack) {
    // Check if the stack has enough elements
    if(stack.empty()) {
        return "'Z' does not work on empty stack";
    }

    // Take head of the stack
    auto head = stack.back();

    // If it's an integer, count its digits
    if(is_num<int>(head)) {
        auto num = std::stoi(head);
        stack.pop_back();

        size_t len = 0;
        while(num > 0) {
            num /= 10;
            len++;
        }

        stack.push_back(std::to_string(len));
    } else {
        // Otherwise, treat the value as a string and count its length
        stack.pop_back();
        head.erase(std::remove(head.begin(), head.end(), '.'), head.end());
        stack.push_back(std::to_string(head.length()));
    }

    return std::nullopt;
}

std::optional<std::string> Stack::fn_stack_size(dc_stack_t &stack) {
    stack.push_back(std::to_string(stack.size()));

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
