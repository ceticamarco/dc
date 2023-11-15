#pragma once

#include "operation.h"

enum class Operator {
    GT, LT, EQ, GEQ, LEQ, NEQ
};

class Macro : public IOperation {
public:
    Macro(const OPType op_t, const Operator o, 
          const char dc_r, std::unordered_map<char, Register> &r, Parameters &p)
        : op_type(std::move(op_t)), op(std::move(o)), 
          dc_register(std::move(dc_r)), regs(r), parameters(p) {}
    Macro(const OPType op_t, std::unordered_map<char, Register> &r, Parameters &p)
        : op_type(std::move(op_t)), regs(r), parameters(p) {}
    std::optional<std::string> exec(dc_stack_t &stack) override;
    static std::vector<std::string> split(std::string str);

private:
    std::optional<std::string> fn_execute(dc_stack_t &stack);
    std::optional<std::string> fn_evaluate_macro(dc_stack_t &stack);
    std::optional<std::string> fn_read_input(dc_stack_t &stack);

    OPType op_type;
    Operator op;
    char dc_register;
    std::unordered_map<char, Register> &regs;
    Parameters parameters;
};
