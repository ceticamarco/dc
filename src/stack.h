#pragma once

#include "operation.h"

class Stack : public IOperation {
public:
    explicit Stack(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc_stack_t &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) override;

private:
    std::optional<std::string> fn_print(dc_stack_t &stack, Parameters  &parameters, bool new_line);
    static std::optional<std::string> fn_pop_head(dc_stack_t &stack);
    static std::optional<std::string> fn_swap_xy(dc_stack_t &stack);
    static std::optional<std::string> fn_dup_head(dc_stack_t &stack);
    std::optional<std::string> fn_print_stack(dc_stack_t &stack, Parameters &parameters);
    static std::optional<std::string> fn_head_size(dc_stack_t &stack);
    static std::optional<std::string> fn_stack_size(dc_stack_t &stack);
    static std::optional<std::string> fn_set_precision(dc_stack_t &stack, Parameters &parameters);
    static std::optional<std::string> fn_get_precision(dc_stack_t &stack, Parameters &parameters);
    static std::optional<std::string> fn_set_oradix(dc_stack_t &stack, Parameters &parameters);
    static std::optional<std::string> fn_get_oradix(dc_stack_t &stack, Parameters &parameters);
    static std::optional<std::string> fn_set_iradix(dc_stack_t &stack, Parameters &parameters);
    static std::optional<std::string> fn_get_iradix(dc_stack_t &stack, Parameters &parameters);
    constexpr std::string to_bin(auto num);

    OPType op_type;
};
