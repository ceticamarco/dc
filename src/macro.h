#pragma once

#include "operation.h"

enum class MacroOP {
    GT, LT, EQ, GEQ, LEQ, NEQ
};

/**
 * @brief Evaluates macro operations
 */
class Macro : public IOperation {
public:
    /**
     * @brief Constructor of Macro
     * 
     * This constructor is being called by the Evaluate class to
     * execute comparison macros
     * @param op_t The identifier of the operation(in this case will always be **OPType::CMP**)
     * @param o The type of comparison operation
     * @param dc_r The name of the register to call when the comparison yields true
     */
    Macro(const OPType op_t, const MacroOP o, const char dc_r) : op_type(op_t), op(o), dc_register(dc_r) {}

    /**
     * Overload of Macro constructor
     * 
     * This constructor is being called by the Evaluate class to 
     * execute general purpose macros commands, that is: executing an element of the
     * stack as a macro, executing a string from stdin and executing from a file
     * @param op_t The identifier of the operation
     */
    explicit Macro(const OPType op_t) : op_type(op_t) {}

    /**
     * @brief Executes the selected operation by switching on the **op_t** identifier
     * 
     * @param stack An instance of the dc::Stack data structure
     * @param parameters An instance of the dc::Parameters data structure
     * @param regs An instance of the dc::Register data structure
     * 
     * @return Runtime errors, if any
     */
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
