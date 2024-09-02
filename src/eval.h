#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <memory>
#include <functional>

#include "adt.h"
#include "operation.h"

/**
 * @brief Class for evaluating DC commands
 *
 *  This class parses and evaluates DC commands. It serves as an entry
 *  point for the whole program.
 */

class Evaluate {
public:
    /**
     * @brief Constructor of Evaluate.
     * @param e The expression to be evaluated
     * @param r An instance of the @ref dc::Register data structure
     * @param s An instance of the @ref dc::Stack data structure
     * @param p An instance of the @ref dc::Parameters data structure
     */
    Evaluate(const std::vector<std::string>& e, std::unordered_map<char, dc::Register> &r,
             dc::Stack<std::string> &s, dc::Parameters &p)
        : expr(e), regs(r), stack(s), parameters(p) {}
    std::optional<std::string> eval();

private:
    std::optional<std::string> parse_macro(std::size_t &idx);
    std::optional<std::string> parse_macro_command(std::string token);
    std::optional<std::string> parse_register_command(std::string token);
    std::optional<std::string> parse_array_command(std::string token);
    std::optional<std::string> parse_base_n(const std::string& token);
    void init_environment();

    using op_factory_t = std::function<std::unique_ptr<IOperation>()>;
    std::vector<std::string> expr;
    std::unordered_map<char, dc::Register> &regs;
    std::unordered_map<std::string, op_factory_t> op_factory;
    dc::Stack<std::string> &stack;
    dc::Parameters &parameters;
};
