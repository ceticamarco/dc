#pragma once

#include "operation.h"

/**
 * @brief Evaluates stack operations
 */
class Bitwise : public IOperation {
public:
    /**
     * @brief Constructor of Bitwise
     * @param op_t The identifier of the operation
     */
    explicit Bitwise(const OPType op_t) : op_type(op_t) {}
    
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
    std::optional<std::string> fn_bitwise_and(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_or(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_not(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_xor(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_lshift(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
    std::optional<std::string> fn_bitwise_rshift(dc::Stack<std::string> &stack, const dc::Parameters &parameters);

    OPType op_type;
};
