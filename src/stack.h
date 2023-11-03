#pragma once

#include "operation.h"

class Stack : public IOperation {
public:
    Stack(const OPType op_type) : op_type(std::move(op_type)) {}
    std::optional<std::string> exec(stack_t &stack) override;

private:
    std::optional<std::string> fn_print(stack_t &stack, bool new_line);
    std::optional<std::string> fn_pop_head(stack_t &stack);
    std::optional<std::string> fn_swap_xy(stack_t &stack);
    std::optional<std::string> fn_dup_head(stack_t &stack);
    std::optional<std::string> fn_print_stack(stack_t &stack);
    std::optional<std::string> fn_head_size(stack_t &stack);
    std::optional<std::string> fn_stack_size(stack_t &stack);

    OPType op_type;
};
