#pragma once
#include <optional>

#include "types.h"

class IOperation {
public:
    virtual std::optional<std::string> exec(dc_stack_t &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) = 0;
    virtual ~IOperation() = default;
};

enum class OPType {
    // Numerical operations
    ADD, SUB, MUL, DIV, MOD, DIV_MOD, MOD_EXP, EXP,
    SQRT, SIN, COS, TAN, ASIN, ACOS, ATAN, FACT, PI, E,
    // Stack operations
    PCG, P, PBB, PBH, PBO, CLR, PH, SO, DP, PS, CH, CS,
    // Macro operations
    EX, CMP, RI
};
