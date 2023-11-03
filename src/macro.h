#pragma once

#include "operation.h"

enum class Operator {
    GT, LT, EQ, GEQ, LEQ, NEQ
};

class Macro : public IOperation {
public:
    Macro(const OPType op_type, const Operator op, const char dc_register, std::unordered_map<char, Register> &regs)
        : op_type(std::move(op_type)), op(std::move(op)), dc_register(std::move(dc_register)), regs(regs) {}
    Macro(const OPType op_type, std::unordered_map<char, Register> &regs)
        : op_type(std::move(op_type)), regs(regs) {}
    std::optional<std::string> exec(stack_t &stack) override;
    static std::vector<std::string> split(std::string str);

private:
    std::optional<std::string> fn_execute(stack_t &stack);
    std::optional<std::string> fn_evaluate_macro(stack_t &stack);
    std::optional<std::string> fn_read_input(stack_t &stack);

    OPType op_type;
    Operator op;
    char dc_register;
    std::unordered_map<char, Register> &regs;
};
