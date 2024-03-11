#pragma once

#include "operation.h"

class Math : public IOperation {
public:
    explicit Math(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) override;

private:
    static std::optional<std::string> fn_add(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_sub(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_mul(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_div(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_mod(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_div_mod(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_mod_exp(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_exp(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_sqrt(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_sin(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_cos(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_tan(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_asin(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_acos(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_atan(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_fact(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_pi(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::optional<std::string> fn_e(DCStack<std::string> &stack, Parameters &parameters) ;
    static std::string trim_digits(double number, unsigned int precision);

    OPType op_type;
};
