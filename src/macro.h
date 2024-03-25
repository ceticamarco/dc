#pragma once

#include "operation.h"

enum class MacroOP {
    GT, LT, EQ, GEQ, LEQ, NEQ
};

class Macro : public IOperation {
public:
    Macro(const OPType op_t, const MacroOP o, const char dc_r) : op_type(op_t), op(o), dc_register(dc_r) {}
    explicit Macro(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;
    static std::vector<std::string> split(const std::string& str);

private:
    static std::optional<std::string> fn_execute(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_evaluate_macro(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    static std::optional<std::string> fn_read_input(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    static std::optional<std::string> fn_evaluate_file(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);

    OPType op_type;
    MacroOP op{};
    char dc_register{};
};
