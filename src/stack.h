#pragma once

#include "operation.h"

class Stack : public IOperation {
public:
    explicit Stack(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;

private:
    std::optional<std::string> fn_print(dc::Stack<std::string> &stack, dc::Parameters  &parameters, bool new_line);
    static std::optional<std::string> fn_pop_head(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_swap_xy(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_dup_head(dc::Stack<std::string> &stack);
    std::optional<std::string> fn_print_stack(const dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_head_size(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_stack_size(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_set_precision(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_get_precision(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_set_oradix(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_get_oradix(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_set_iradix(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_get_iradix(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_get_lastx(dc::Stack<std::string> &stack);
    std::optional<std::string> fn_get_lasty(dc::Stack<std::string> &stack);
    std::optional<std::string> fn_get_lastz(dc::Stack<std::string> &stack);
    std::string bin_prettify(std::string s);

    OPType op_type;
};
