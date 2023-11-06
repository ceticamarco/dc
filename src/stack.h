#pragma once

#include "operation.h"

class Stack : public IOperation {
public:
    Stack(const OPType op_t) : op_type(std::move(op_t)) {}
    std::optional<std::string> exec(dc_stack_t &stack) override;

private:
    std::optional<std::string> fn_print(dc_stack_t &stack, bool new_line);
    std::optional<std::string> fn_pop_head(dc_stack_t &stack);
    std::optional<std::string> fn_swap_xy(dc_stack_t &stack);
    std::optional<std::string> fn_dup_head(dc_stack_t &stack);
    std::optional<std::string> fn_print_stack(dc_stack_t &stack);
    std::optional<std::string> fn_head_size(dc_stack_t &stack);
    std::optional<std::string> fn_stack_size(dc_stack_t &stack);

    OPType op_type;
};
