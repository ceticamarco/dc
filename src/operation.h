#pragma once
#include <optional>

#include "adt.h"

class IOperation {
public:
    virtual std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) = 0;
    virtual ~IOperation() = default;
};

enum class OPType {
    // Numerical operations
    ADD, SUB, MUL, DIV, MOD, DIV_MOD, MOD_EXP, EXP,
    SQRT, SIN, COS, TAN, ASIN, ACOS, ATAN, FACT, PI, 
    E, RND, INT, TO_CMPLX, GET_RE, GET_IM, LOG,
    // Statistical operations
    PERM, COMB, SUMX, SUMXX, MEAN, SDEV, LREG,
    // Bitwise operations
    BAND, BOR, BNOT, BXOR, BSL, BSR,
    // Stack operations
    PCG, PWS, P, PBB, PBH, PBO, CLR, PH, SO, DP, PS, CH, CS, 
    SP, GP, SOR, GOR, SIR, GIR, LX, LY, LZ,
    // Macro operations
    EX, CMP, RI, LF
};
