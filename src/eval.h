#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "types.h"

class Evaluate {
public:
    Evaluate(const std::vector<std::string> e, std::unordered_map<char, Register> &r, dc_stack_t &s)
        : expr(std::move(e)), regs(r), stack(s) {}
    Evaluate(std::unordered_map<char, Register> &r, dc_stack_t &s)
        : regs(r), stack(s) {}
    std::optional<std::string> eval();

private:
    std::optional<std::string> handle_special(std::string val, size_t &idx);
    std::optional<std::string> parse_macro(size_t &idx);
    std::optional<std::string> parse_macro_command(std::string val);
    std::optional<std::string> parse_register_command(std::string val);
    std::optional<std::string> parse_array_command(std::string val);

    std::vector<std::string> expr;
    std::unordered_map<char, Register> &regs;
    dc_stack_t &stack;
};
