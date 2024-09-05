#include <iostream>
#include <sstream>
#include <iterator>
#include <limits>
#include <fstream>

#include "adt.cpp"
#include "eval.h"
#include "macro.h"
#include "num_utils.h"

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

/**
 * @brief Executes the head of the stack as a macro
 * 
 * Takes one value from the stack and executes it as a macro if it is a string. If the value
 * is a number, pushes it back onto the stack
 * 
 * @param stack An instance of dc::Stack
 * @param parameters An instance of dc::Parameters
 * @param regs An instance of the dc::Register
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Macro::fn_execute(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check if stack has enough elements
    if(stack.empty()) {
        return "This operation does not work on empty stack";
    }

    // If the head of the stack is a string
    // pop it and execute it as a macro
    auto head = stack.pop(false);
    if(!NumericUtils::is_numeric<double>(head)) {
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

/**
 * @brief Executes a comparison operation
 * 
 * Takes two values from the stack and compares them. If the top-of-stack
 * is greater than the second-to-top of the stack, executes the content
 * of the register specified by Macro::op as a macro
 * 
 * @param stack An instance of dc::Stack
 * @param parameters An instance of dc::Parameters
 * @param regs An instance of the dc::Register
 * 
 * @return Evaluation errors, if any
 */
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
    if(!dc_macro.empty() && NumericUtils::is_numeric<double>(head_str) && NumericUtils::is_numeric<double>(second_str)) {
        auto head = std::stod(head_str);
        auto second = std::stod(second_str);

        switch(this->op) {
            case MacroOP::GT: {
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
            case MacroOP::LT: {
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
            case MacroOP::EQ: {
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
            case MacroOP::GEQ: {
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
            case MacroOP::LEQ: {
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
            case MacroOP::NEQ: {
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

/**
 * @brief Executes a macro from standard input
 * 
 * Reads a string from standard input and executes it as a macro
 * 
 * @param stack An instance of dc::Stack
 * @param parameters An instance of dc::Parameters
 * @param regs An instance of the dc::Register
 * 
 * @return Evaluation errors, if any
 */
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

/**
 * @brief Executes the content of a file
 * 
 * Takes a string from the stack and uses it as a filepath, then reads its content
 * and executes it as a macro
 * 
 * @param stack An instance of dc::Stack
 * @param parameters An instance of dc::Parameters
 * @param regs An instance of the dc::Register
 * 
 * @return Evaluation errors, if any
 */
std::optional<std::string> Macro::fn_evaluate_file(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) {
    // Check whether the main stack has enough elements
    if(stack.empty()) {
        return "This operation does not work on empty stack";
    }

    // If the head of the stack is a string,
    auto file_name = stack.pop(false);
    if(!NumericUtils::is_numeric<double>(file_name)) {
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
