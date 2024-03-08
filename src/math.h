#pragma once

#include "operation.h"

class Math : public IOperation {
public:
    explicit Math(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc_stack_t &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) override;

private:
    static std::optional<std::string> fn_add(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_sub(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_mul(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_div(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_mod(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_div_mod(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_mod_exp(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_exp(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_sqrt(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_sin(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_cos(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_tan(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_asin(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_acos(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_atan(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_fact(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_pi(dc_stack_t &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_e(dc_stack_t &stack, Parameters &parameters) ;
    static std::string trim_digits(double number, unsigned int precision);

    OPType op_type;
};
