#pragma once
#include <optional>

#include "types.h"

class IOperation {
public:
    virtual std::optional<std::string> exec(stack_t &stack) = 0;
    virtual ~IOperation() = default;
};

enum class OPType {
    // Numerical operations
    ADD, SUB, MUL, DIV, MOD, DIV_MOD, MOD_EXP, EXP,
    SQRT, SIN, COS, TAN, FACT, PI, E,
    // Stack operations
    PCG, P, CLR, PH, SO, DP, PS, CH, CS,
    // Macro operations
    EX, CMP, RI
};
