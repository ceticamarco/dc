#pragma once

#include "operation.h"

class Statistics : public IOperation {
public:
    explicit Statistics(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;

private:
    std::optional<std::string> fn_perm(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_comb(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_sum(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_sum_squared(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_mean(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_sdev(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_lreg(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<unsigned long long> factorial(const long long n);
    std::string trim_digits(double number, unsigned int precision);

    OPType op_type;
};
