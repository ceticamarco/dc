#include <iostream>
#include <sstream>
#include <iterator>
#include <limits>
#include <fstream>

#include "adt.cpp"
#include "eval.h"
#include "macro.h"
#include "is_num.h"

std::optional<std::string> Macro::exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        case OPType::EX: err = fn_execute(stack, parameters, regs); break;
        case OPType::CMP: err = fn_evaluate_macro(stack, parameters, regs); break;
        case OPType::RI: err = fn_read_input(stack, parameters, regs); break;
        case OPType::LF: err = fn_evaluate_file(stack, parameters, regs); break;
        default: break;
    }

    return err;
}

std::optional<std::string> Macro::fn_execute(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "This operation does not work on empty stack";
    }

    // If the head of the stack is a string
    // pop it and execute it as a macro
    auto head = stack.pop(false);
    if(!is_num<double>(head)) {
        stack.copy_xyz();
        stack.pop(true);
        std::vector<std::string> tokens = split(head);
        Evaluate evaluator(tokens, regs, stack, parameters);
        
        auto err = evaluator.eval();
        if(err != std::nullopt) {
            return err;
        }
    }

    return std::nullopt;
}

std::optional<std::string> Macro::fn_evaluate_macro(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether the main stack has enough elements
    if(stack.size() < 2) {
        return "This operation requires two elements";
    }

    // Check whether the register's stack exists or not
    if(regs.find(this->dc_register) == regs.end()) {
        return "Null register";
    }

    // Extract macro and top two values of the stack
    stack.copy_xyz();
    auto head_str = stack.pop(true);
    auto second_str = stack.pop(true);
    auto dc_macro = regs[this->dc_register].stack.pop(false);

    // Check if macro exists and if top two elements of main stack are numbers
    if(!dc_macro.empty() && is_num<double>(head_str) && is_num<double>(second_str)) {
        auto head = std::stod(head_str);
        auto second = std::stod(second_str);

        switch(this->op) {
            case Operator::GT: {
                if(head > second) {
                    std::vector<std::string> tokens = split(dc_macro);
                    Evaluate evaluator(tokens, regs, stack, parameters);

                    auto err = evaluator.eval();
                    if(err != std::nullopt) {
                        return err;
                    }
                }
                break;
            }
            case Operator::LT: {
                if(head < second) {
                    std::vector<std::string> tokens = split(dc_macro);
                    Evaluate evaluator(tokens, regs, stack, parameters);

                    auto err = evaluator.eval();
                    if(err != std::nullopt) {
                        return err;
                    }
                }
                break;
            }
            case Operator::EQ: {
                if(head == second) {
                    std::vector<std::string> tokens = split(dc_macro);
                    Evaluate evaluator(tokens, regs, stack, parameters);

                    auto err = evaluator.eval();
                    if(err != std::nullopt) {
                        return err;
                    }
                }
                break;
            }
            case Operator::GEQ: {
                if(head >= second) {
                    std::vector<std::string> tokens = split(dc_macro);
                    Evaluate evaluator(tokens, regs, stack, parameters);

                    auto err = evaluator.eval();
                    if(err != std::nullopt) {
                        return err;
                    }
                }
                break;
            }
            case Operator::LEQ: {
                if(head <= second) {
                    std::vector<std::string> tokens = split(dc_macro);
                    Evaluate evaluator(tokens, regs, stack, parameters);

                    auto err = evaluator.eval();
                    if(err != std::nullopt) {
                        return err;
                    }
                }
                break;
            }
            case Operator::NEQ: {
                if(head != second) {
                    std::vector<std::string> tokens = split(dc_macro);
                    Evaluate evaluator(tokens, regs, stack, parameters);

                    auto err = evaluator.eval();
                    if(err != std::nullopt) {
                        return err;
                    }
                }
                break;
            }
        }
    }

    return std::nullopt;
}

std::optional<std::string> Macro::fn_read_input(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Read user input from stdin
    std::string user_input;

    std::getline(std::cin, user_input);
    if(std::cin.fail()) {
        return "Error while reading from stdin";
    }

    // Push the input onto the main stack and execute it as a macro
    std::vector<std::string> tokens = split(user_input);
    Evaluate evaluator(tokens, regs, stack, parameters);
        
    auto err = evaluator.eval();
    if(err != std::nullopt) {
        return err;
    }

    return std::nullopt;
}

std::optional<std::string> Macro::fn_evaluate_file(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether the main stack has enough elements
    if(stack.empty()) {
        return "This operation does not work on empty stack";
    }

    // If the head of the stack is a string,
    auto file_name = stack.pop(false);
    if(!is_num<double>(file_name)) {
        // Pop it from the stack
        stack.copy_xyz();
        stack.pop(true);
        // And use it as a filename
        std::fstream source_file(file_name, std::ios::in | std::ios::binary);
        if(source_file.fail()) {
            return std::string("Cannot open source file \"") + file_name + std::string("\"");
        }
        
        // Read whole file into a buffer
        std::stringstream buf;
        buf << source_file.rdbuf();

        // Execute file line by line
        std::string line;
        while(std::getline(buf, line, '\n')) {
            // Ignore comments or empty lines
            if(line.empty() || line.starts_with('#')) {
                continue;
            }

            // Remove inline comments
            auto comment_pos = line.find('#');
            std::vector<std::string> tokens;

            if(comment_pos != std::string::npos) {
                // Convert only the first part of the line
                tokens = split(line.substr(0, comment_pos));
            } else {
                // Otherwise split the whole string
                tokens = split(line);
            }

            // Evaluate expression
            Evaluate evaluator(tokens, regs, stack, parameters);
            auto err = evaluator.eval();
            // Handle errors
            if(err != std::nullopt) {
                return err;
            }
        }
    } else {
        return "This operation requires string values";
    }

    return std::nullopt;
}

std::vector<std::string> Macro::split(const std::string& str) {
    std::stringstream ss(str);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vec(begin, end);

    return vec;
}
