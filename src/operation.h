#pragma once
#include <optional>

#include "adt.h"
/**
 * @brief This protocol establishes a set of methods to which every DC operation(represented by a class) must adhere.
 * 
 */
class IOperation {
public:
    /**
     * @brief Executes a DC operation
     * 
     * @param stack An instance of the dc::Stack data structure
     * @param parameters An instance of the dc::Parameters data structure
     * @param regs An instance of the dc::Register data structure
     * 
     * @return Runtime errors, if any
     */
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
