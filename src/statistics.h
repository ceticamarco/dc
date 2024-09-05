#pragma once

#include "operation.h"

/**
 * @brief Evaluates statistics operations
 */
class Statistics : public IOperation {
public:
    /**
     * @brief Constructor of Statistics
     * @param op_t The identifier of the operation
     */
    explicit Statistics(const OPType op_t) : op_type(op_t) {}

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
    std::optional<std::string> fn_perm(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_comb(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_sum(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_sum_squared(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_mean(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_sdev(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);
    std::optional<std::string> fn_lreg(dc::Stack<std::string> &stack, const dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs);

    OPType op_type;
};
