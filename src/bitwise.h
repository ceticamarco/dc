#pragma once

#include "operation.h"

class Bitwise : public IOperation {
public:
    explicit Bitwise(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;

private:
    std::optional<std::string> fn_bitwise_and(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_or(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_not(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_xor(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_lshift(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_rshift(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::string trim_digits(double number, unsigned int precision);

    OPType op_type;
};
