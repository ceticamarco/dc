#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "types.h"

class Evaluate {
public:
    Evaluate(const std::vector<std::string>& e, std::unordered_map<char, Register> &r,
            dc_stack_t &s, Parameters &p)
        : expr(e), regs(r), stack(s), parameters(p) {}
    Evaluate(std::unordered_map<char, Register> &r, dc_stack_t &s, Parameters &p)
        : regs(r), stack(s), parameters(p) {}
    std::optional<std::string> eval();

private:
    std::optional<std::string> handle_special(std::string val, size_t &idx);
    std::optional<std::string> parse_macro(size_t &idx);
    std::optional<std::string> parse_macro_command(std::string val);
    std::optional<std::string> parse_register_command(std::string val);
    std::optional<std::string> parse_array_command(std::string val);
    std::optional<std::string> parse_base_n(const std::string& val);
    std::optional<std::string> fn_set_precision();
    std::optional<std::string> fn_get_precision();
    std::optional<std::string> fn_set_oradix();
    std::optional<std::string> fn_get_oradix();
    std::optional<std::string> fn_set_iradix();
    std::optional<std::string> fn_get_iradix();

    std::vector<std::string> expr;
    std::unordered_map<char, Register> &regs;
    dc_stack_t &stack;
    Parameters &parameters;
};
