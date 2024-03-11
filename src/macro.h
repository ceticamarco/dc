#pragma once

#include "operation.h"

enum class Operator {
    GT, LT, EQ, GEQ, LEQ, NEQ
};

class Macro : public IOperation {
public:
    Macro(const OPType op_t, const Operator o, const char dc_r) : op_type(op_t), op(o), dc_register(dc_r) {}
    explicit Macro(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) override;
    static std::vector<std::string> split(const std::string& str);

private:
    static std::optional<std::string> fn_execute(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs);
    std::optional<std::string> fn_evaluate_macro(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs);
    static std::optional<std::string> fn_read_input(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs);

    OPType op_type;
    Operator op{};
    char dc_register{};
};
