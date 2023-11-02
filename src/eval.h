#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "types.h"

class Evaluate {
public:
    Evaluate(std::vector<std::string> expr, std::unordered_map<char, Register> &regs, stack_t &stack)
        : expr(expr), regs(regs), stack(stack) {}
    Evaluate(std::unordered_map<char, Register> &regs, stack_t &stack)
        : regs(regs), stack(stack) {}
    std::optional<std::string> eval();

private:
    std::optional<std::string> handle_special(std::string val, size_t &idx, std::vector<std::string> &expr);
    std::optional<std::string> parse_macro(size_t &idx, std::vector<std::string> &expr);
    std::optional<std::string> parse_macro_command(std::string val);
    std::optional<std::string> parse_register_command(std::string val);
    std::optional<std::string> parse_array_command(std::string val);

    std::vector<std::string> expr;
    std::unordered_map<char, Register> &regs;
    stack_t &stack;
};
