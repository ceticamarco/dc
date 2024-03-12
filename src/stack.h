#pragma once

#include "operation.h"

class Stack : public IOperation {
public:
    explicit Stack(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) override;

private:
    std::optional<std::string> fn_print(DCStack<std::string> &stack, Parameters  &parameters, bool new_line);
    static std::optional<std::string> fn_pop_head(DCStack<std::string> &stack);
    static std::optional<std::string> fn_swap_xy(DCStack<std::string> &stack);
    static std::optional<std::string> fn_dup_head(DCStack<std::string> &stack);
    std::optional<std::string> fn_print_stack(DCStack<std::string> &stack, Parameters &parameters);
    static std::optional<std::string> fn_head_size(DCStack<std::string> &stack);
    static std::optional<std::string> fn_stack_size(DCStack<std::string> &stack);
    static std::optional<std::string> fn_set_precision(DCStack<std::string> &stack, Parameters &parameters);
    static std::optional<std::string> fn_get_precision(DCStack<std::string> &stack, Parameters &parameters);
    static std::optional<std::string> fn_set_oradix(DCStack<std::string> &stack, Parameters &parameters);
    static std::optional<std::string> fn_get_oradix(DCStack<std::string> &stack, Parameters &parameters);
    static std::optional<std::string> fn_set_iradix(DCStack<std::string> &stack, Parameters &parameters);
    static std::optional<std::string> fn_get_iradix(DCStack<std::string> &stack, Parameters &parameters);
    std::optional<std::string> fn_get_lastx(DCStack<std::string> &stack);
    std::optional<std::string> fn_get_lasty(DCStack<std::string> &stack);
    std::optional<std::string> fn_get_lastz(DCStack<std::string> &stack);
    constexpr std::string to_bin(auto num);

    OPType op_type;
};
