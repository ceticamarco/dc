#pragma once

#include "operation.h"

class Mathematics : public IOperation {
public:
    explicit Mathematics(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;

private:
    static std::optional<std::string> fn_add(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_sub(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_mul(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_div(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_mod(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_div_mod(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_mod_exp(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_exp(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_sqrt(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_sin(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_cos(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_tan(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_asin(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_acos(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_atan(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_fact(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_pi(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_e(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_random(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_integer(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_to_complex(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_get_real(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_get_imaginary(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_log10(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::string trim_digits(double number, unsigned int precision);
    static bool is_complex(const std::string &str);

    OPType op_type;
};
