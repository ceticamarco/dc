#pragma once

#include "operation.h"

enum class StackOP {
    P, PNL, PS
};

/**
 * @brief Evaluates stack operations
 */
class Stack : public IOperation {
public:
    /**
     * @brief Constructor of Stack
     * @param op_t The identifier of the operation
     */
    explicit Stack(const OPType op_t) : op_type(op_t) {}

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

private:
    std::optional<std::string> fn_print(dc::Stack<std::string> &stack, dc::Parameters  &parameters, const StackOP op);
    static std::optional<std::string> fn_pop_head(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_swap_xy(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_dup_head(dc::Stack<std::string> &stack);
    std::optional<std::string> fn_print_stack(const dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_head_size(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_stack_size(dc::Stack<std::string> &stack);
    static std::optional<std::string> fn_set_precision(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_get_precision(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    static std::optional<std::string> fn_set_oradix(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_get_oradix(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_set_iradix(dc::Stack<std::string> &stack, dc::Parameters &parameters);
    static std::optional<std::string> fn_get_iradix(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_get_lastx(dc::Stack<std::string> &stack);
    std::optional<std::string> fn_get_lasty(dc::Stack<std::string> &stack);
    std::optional<std::string> fn_get_lastz(dc::Stack<std::string> &stack);
    std::string bin_prettify(std::string s);

    OPType op_type;
};
