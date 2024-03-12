#pragma once
#include <optional>

#include "adt.h"

class IOperation {
public:
    virtual std::optional<std::string> exec(DCStack<std::string> &stack, Parameters &parameters, std::unordered_map<char, Register> &regs) = 0;
    virtual ~IOperation() = default;
};

enum class OPType {
    // Numerical operations
    ADD, SUB, MUL, DIV, MOD, DIV_MOD, MOD_EXP, EXP,
    SQRT, SIN, COS, TAN, ASIN, ACOS, ATAN, FACT, PI, E,
    // Stack operations
    PCG, P, PBB, PBH, PBO, CLR, PH, SO, DP, PS, CH, CS, 
    SP, GP, SOR, GOR, SIR, GIR, LX, LY, LZ,
    // Macro operations
    EX, CMP, RI
};
