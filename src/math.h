#pragma once

#include "operation.h"

class Math : public IOperation {
public:
    Math(const OPType op_t, const unsigned int p) : op_type(std::move(op_t)), precision(std::move(p)) {}
    std::optional<std::string> exec(dc_stack_t &stack) override;

private:
    std::optional<std::string> fn_add(dc_stack_t &stack);
    std::optional<std::string> fn_sub(dc_stack_t &stack);
    std::optional<std::string> fn_mul(dc_stack_t &stack);
    std::optional<std::string> fn_div(dc_stack_t &stack);
    std::optional<std::string> fn_mod(dc_stack_t &stack);
    std::optional<std::string> fn_div_mod(dc_stack_t &stack);
    std::optional<std::string> fn_mod_exp(dc_stack_t &stack);
    std::optional<std::string> fn_exp(dc_stack_t &stack);
    std::optional<std::string> fn_sqrt(dc_stack_t &stack);
    std::optional<std::string> fn_sin(dc_stack_t &stack);
    std::optional<std::string> fn_cos(dc_stack_t &stack);
    std::optional<std::string> fn_tan(dc_stack_t &stack);
    std::optional<std::string> fn_fact(dc_stack_t &stack);
    std::optional<std::string> fn_pi(dc_stack_t &stack);
    std::optional<std::string> fn_e(dc_stack_t &stack);
    static std::string trim_digits(double number, unsigned int precision);

    OPType op_type;
    unsigned int precision;
};
