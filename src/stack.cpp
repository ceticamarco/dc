#include <iostream>
#include <algorithm>

#include "stack.h"
#include "is_num.h"

std::optional<std::string> Stack::exec(dc_stack_t &stack) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        case OPType::PCG: err = fn_print(stack, true); break;
        case OPType::P: err = fn_print(stack, false); break;
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

    if(new_line) {
        std::cout << stack.back() << std::endl;
    } else {
        std::cout << stack.back();
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
    for(auto it = stack.rbegin(); it != stack.rend(); it++) {
        std::cout << *it << std::endl;
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
