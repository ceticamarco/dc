#pragma once

#include "operation.h"

enum class Operator {
    GT, LT, EQ, GEQ, LEQ, NEQ
};

class Macro : public IOperation {
public:
    Macro(OPType op_type, Operator op, char dc_register, std::unordered_map<char, Register> &regs)
        : op_type(op_type), op(op), dc_register(dc_register), regs(regs) {}
    Macro(OPType op_type, std::unordered_map<char, Register> &regs)
        : op_type(op_type), regs(regs) {}
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
