#pragma once

#include "operation.h"

class Math : public IOperation {
public:
    Math(OPType op_type) : op_type(op_type) {}
    std::optional<std::string> exec(stack_t &stack) override;

private:
    std::optional<std::string> fn_add(stack_t &stack);
    std::optional<std::string> fn_sub(stack_t &stack);
    std::optional<std::string> fn_mul(stack_t &stack);
    std::optional<std::string> fn_div(stack_t &stack);
    std::optional<std::string> fn_mod(stack_t &stack);
    std::optional<std::string> fn_div_mod(stack_t &stack);
    std::optional<std::string> fn_mod_exp(stack_t &stack);
    std::optional<std::string> fn_exp(stack_t &stack);
    std::optional<std::string> fn_sqrt(stack_t &stack);
    std::optional<std::string> fn_sin(stack_t &stack);
    std::optional<std::string> fn_cos(stack_t &stack);
    std::optional<std::string> fn_tan(stack_t &stack);
    std::optional<std::string> fn_fact(stack_t &stack);
    std::optional<std::string> fn_pi(stack_t &stack);
    std::optional<std::string> fn_e(stack_t &stack);
    static std::string trim_zeros(double number);

    OPType op_type;
};
