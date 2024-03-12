#include <memory>

#include "adt.cpp"
#include "eval.h"
#include "math.h"
#include "stack.h"
#include "macro.h"
#include "is_num.h"

#define MACRO_COND(VAL) ((VAL.length() == 1 && VAL == "["))
#define MACRO_CMD_COND(VAL) ((VAL.length() == 2 || VAL.length() == 3) && \
              (VAL.at(0) == '>' || VAL.at(0) == '<' || \
               VAL.at(0) == '=' || VAL.at(0) == '!'))
#define REGISTER_COND(VAL) ((VAL.length() == 2) && \
              (VAL.at(0) == 's' || VAL.at(0) == 'S' || \
               VAL.at(0) == 'l' || VAL.at(0) == 'L'))
#define ARRAY_COND(VAL) ((VAL.length() == 2) && \
        (VAL.at(0) == ':' || VAL.at(0) == ';'))

#define X_CONTAINS_Y(X, Y) ((Y.find_first_of(X) != std::string::npos))
#define MAKE_UNIQUE_PTR(CLASS, ARG) [](){ return std::make_unique<CLASS>(ARG); }

void Evaluate::init_environment() {
    // Numerical operations
    this->op_factory.emplace("+", MAKE_UNIQUE_PTR(Math, OPType::ADD));
    this->op_factory.emplace("-", MAKE_UNIQUE_PTR(Math, OPType::SUB));
    this->op_factory.emplace("*", MAKE_UNIQUE_PTR(Math, OPType::MUL));
    this->op_factory.emplace("/", MAKE_UNIQUE_PTR(Math, OPType::DIV));
    this->op_factory.emplace("%", MAKE_UNIQUE_PTR(Math, OPType::MOD));
    this->op_factory.emplace("~", MAKE_UNIQUE_PTR(Math, OPType::DIV_MOD));
    this->op_factory.emplace("|", MAKE_UNIQUE_PTR(Math, OPType::MOD_EXP));
    this->op_factory.emplace("^", MAKE_UNIQUE_PTR(Math, OPType::EXP));
    this->op_factory.emplace("v", MAKE_UNIQUE_PTR(Math, OPType::SQRT));
    this->op_factory.emplace("sin", MAKE_UNIQUE_PTR(Math, OPType::SIN));
    this->op_factory.emplace("cos", MAKE_UNIQUE_PTR(Math, OPType::COS));
    this->op_factory.emplace("tan", MAKE_UNIQUE_PTR(Math, OPType::TAN));
    this->op_factory.emplace("asin", MAKE_UNIQUE_PTR(Math, OPType::ASIN));
    this->op_factory.emplace("acos", MAKE_UNIQUE_PTR(Math, OPType::ACOS));
    this->op_factory.emplace("atan", MAKE_UNIQUE_PTR(Math, OPType::ATAN));
    this->op_factory.emplace("!", MAKE_UNIQUE_PTR(Math, OPType::FACT));
    this->op_factory.emplace("pi", MAKE_UNIQUE_PTR(Math, OPType::PI));
    this->op_factory.emplace("e", MAKE_UNIQUE_PTR(Math, OPType::E));
    // Stack operations
    this->op_factory.emplace("p", MAKE_UNIQUE_PTR(Stack, OPType::PCG));
    this->op_factory.emplace("pb", MAKE_UNIQUE_PTR(Stack, OPType::PBB));
    this->op_factory.emplace("ph", MAKE_UNIQUE_PTR(Stack, OPType::PBH));
    this->op_factory.emplace("po", MAKE_UNIQUE_PTR(Stack, OPType::PBO));
    this->op_factory.emplace("P", MAKE_UNIQUE_PTR(Stack, OPType::P));
    this->op_factory.emplace("c", MAKE_UNIQUE_PTR(Stack, OPType::CLR));
    this->op_factory.emplace("R", MAKE_UNIQUE_PTR(Stack, OPType::PH));
    this->op_factory.emplace("r", MAKE_UNIQUE_PTR(Stack, OPType::SO));
    this->op_factory.emplace("d", MAKE_UNIQUE_PTR(Stack, OPType::DP));
    this->op_factory.emplace("f", MAKE_UNIQUE_PTR(Stack, OPType::PS));
    this->op_factory.emplace("Z", MAKE_UNIQUE_PTR(Stack, OPType::CH));
    this->op_factory.emplace("z", MAKE_UNIQUE_PTR(Stack, OPType::CS));
    this->op_factory.emplace("k", MAKE_UNIQUE_PTR(Stack, OPType::SP));
    this->op_factory.emplace("K", MAKE_UNIQUE_PTR(Stack, OPType::GP));
    this->op_factory.emplace("o", MAKE_UNIQUE_PTR(Stack, OPType::SOR));
    this->op_factory.emplace("O", MAKE_UNIQUE_PTR(Stack, OPType::GOR));
    this->op_factory.emplace("i", MAKE_UNIQUE_PTR(Stack, OPType::SIR));
    this->op_factory.emplace("I", MAKE_UNIQUE_PTR(Stack, OPType::GIR));
    this->op_factory.emplace(".x", MAKE_UNIQUE_PTR(Stack, OPType::LX));
    this->op_factory.emplace(".y", MAKE_UNIQUE_PTR(Stack, OPType::LY));
    this->op_factory.emplace(".z", MAKE_UNIQUE_PTR(Stack, OPType::LZ));
    // Macro operations
    this->op_factory.emplace("x", MAKE_UNIQUE_PTR(Macro, OPType::EX));
    this->op_factory.emplace("?", MAKE_UNIQUE_PTR(Macro, OPType::RI));
}

std::optional<std::string> Evaluate::eval() {
    // Set up environment
    init_environment();

    for(size_t idx = 0; idx < this->expr.size(); idx++) {
        std::optional<std::string> err = std::nullopt;
        auto token = this->expr.at(idx);

        // If token exists in the environment, instantiate it through the factory
        if (this->op_factory.find(token) != this->op_factory.end()) {
            std::unique_ptr<IOperation> operation = this->op_factory[token]();
            err = operation->exec(this->stack, this->parameters, this->regs);
        } else if(MACRO_COND(token)) {
            err = parse_macro(idx);
        } else if(MACRO_CMD_COND(token)) {
            err = parse_macro_command(token);
        } else if(REGISTER_COND(token)) {
            err = parse_register_command(token);
        } else if(ARRAY_COND(token)) {
            err = parse_array_command(token);
        } else if(this->parameters.iradix != 10) {
            err = parse_base_n(token);
        } else if(is_num<double>(token)) {
            this->stack.push(token);
        } else if(token == "q") {
            std::exit(0);
        } else {
            return "Unrecognized option";
        }

        if(err != std::nullopt) {
            return err;
        }
    }

    return std::nullopt;
}

std::optional<std::string> Evaluate::parse_base_n(const std::string& token) {
    // Discard values that are neither integers neither within "ABCDEF"
    if(!is_num<long>(token) && !X_CONTAINS_Y("ABCDEF", token)) {
        return "This input base supports integers only";
    }

    // Try to convert the number to the selected numeric base
    try {
        long number = std::stol(token, nullptr, this->parameters.iradix);
        this->stack.push(std::to_string(number));
    } catch(...) {
        return "Invalid number for input base '" 
            + std::to_string(this->parameters.iradix) + "'"; 
    }

    return std::nullopt;
}

std::optional<std::string> Evaluate::parse_macro(size_t &idx) {
    // A macro is any string surrounded by square brackets
    std::string dc_macro;
    bool closing_bracket = false;

    // Scan next token
    idx++;

    // Parse the macro
    while(idx < this->expr.size()) {
        // Continue to parse until the clsoing square brackets
        if(this->expr.at(idx) == "]") {
            closing_bracket = true;
            break;
        }

        // Otherwise append the token to the macro.
        // If the macro is not empty, add some spacing
        // before the new token
        if(dc_macro.empty()) {
            dc_macro += this->expr.at(idx);
        } else {
            dc_macro += ' ' + this->expr.at(idx);
        }

        // Go to the next token
        idx++;
    }

    // Check if macro is properly formatted
    if(!closing_bracket) {
        return "Unbalanced parenthesis";
    }

    // Check if macro is empty
    if(dc_macro.empty()) {
        return "Empty macro";
    }

    // Push the macro back onto the stack
    this->stack.push(dc_macro);

    return std::nullopt;
}

std::optional<std::string> Evaluate::parse_macro_command(std::string token) {
    // A macro command is a comparison symbol(>, <, =, >=, <=, !=)
    // followed by a register name(e.g, >A)
    // If command has length equal to three, then it's either '<=', '>=' or '!='

    // Check if command is >=, <= or !=
    std::string operation;
    char dc_register = 0;
    if(token.length() == 3) {
        operation = token.substr(0, 2);
        dc_register = token.at(2);
    } else { // Otherwise it's either >, < or =
        operation = token.at(0);
        dc_register = token.at(1);
    }

    // Macro commands works as follows
    // Pop two values off the stack and compares them assuming
    // they are numbers. If top-of-stack is greater,
    // execute register's content as a macro
    std::optional<std::string> err = std::nullopt;
    if(operation == ">") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::GT, dc_register);
        err = macro->exec(this->stack, this->parameters, this->regs);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "<") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::LT, dc_register);
        err = macro->exec(this->stack, this->parameters, this->regs);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::EQ, dc_register);
        err = macro->exec(this->stack, this->parameters, this->regs);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == ">=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::GEQ, dc_register);
        err = macro->exec(this->stack, this->parameters, this->regs);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "<=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::LEQ, dc_register);
        err = macro->exec(this->stack, this->parameters, this->regs);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "!=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::NEQ, dc_register);
        err = macro->exec(this->stack, this->parameters, this->regs);
        if(err != std::nullopt) {
            return err;
        }
    }

    return err;
}

std::optional<std::string> Evaluate::parse_register_command(std::string token) {
    // A register command has length equal to 2
    // and starts either with 's', 'l'(i.e. "sX" or "lX")
    // or with 'S' or 'L'(i.e., "SX", "LX")
    if(token.at(0) == 's') {
        // Check if main stack is empty
        if(this->stack.empty()) {
            return "This operation does not work on empty stack";
        }

        // Otherwise pop an element from main stack and store it into
        // the register's top-of-the-stack. Any previous value gets overwritten
        this->stack.copy_xyz();
        auto reg_name = token.at(1);
        auto head = this->stack.pop(true);

        // Always discard previous instance of the register
        // i.e., initialize a new instance of register 'reg_name'
        this->regs.erase(reg_name);
        this->regs.insert(
                std::make_pair(reg_name, Register{
                    DCStack<std::string>(),
                    std::unordered_map<int, std::string>()
                })
        );

         // Populate register's 'reg_name' stack with top of main stack
         this->regs[reg_name].stack.push(head);
    } else if(token.at(0) == 'S') {
        // An uppercase 'S' pops the top of the main stack and
        // pushes it onto the stack of selected register.
        // The previous value of the register's stack becomes
        // inaccessible(i.e. it follows LIFO policy).

        // Check if main stack is empty
        if(this->stack.empty()) {
            return "This operation does not work on empty stack";
        }

        this->stack.copy_xyz();
        auto reg_name = token.at(1);
        auto head = this->stack.pop(true);

        // If register's stack exist, push an element onto its stack
        // otherwise allocate a new instance of the register
        auto it = this->regs.find(reg_name);
        if(it != this->regs.end()) { // Register exists
            it->second.stack.push(head);
        } else { // Register doesn't exist
            this->regs[reg_name] = Register{
                DCStack<std::string>(),
                std::unordered_map<int, std::string>()
            };
        }
    } else if(token.at(0) == 'L') {
        // An uppercase 'L' pops the top of the register's stack
	    // abd pushes it onto the main stack. The previous register's stack
    	// value, if any, is accessible via the lowercase 'l' command
        auto reg_name = token.at(1);

        // Check if register exists
        if(this->regs.find(reg_name) == this->regs.end()) {
            return std::string("Register '") + reg_name + std::string("' is undefined");
        }

        // Check if register's stack is empty
        if(this->regs[reg_name].stack.empty()) {
            return std::string("The stack of register '") + reg_name + std::string("' is empty");
        }

        // Otherwise, pop an element from the register's stack and push it onto the main stack
        auto value = this->regs[reg_name].stack.pop(true);
        this->stack.push(value);
    } else {
        // Otherwise retrieve the register name and push its value
    	// to the stack without altering the register's stack.
	    // If the register is empty, push '0' to the stack
        auto reg_name = token.at(1);

        // If register does not exist or its stack is empty, push '0' onto the main stack
        auto it = this->regs.find(reg_name);
        if(it == this->regs.end() || it->second.stack.empty()) {
            this->stack.push("0");
            return std::nullopt;
        }

        // Otherwise, peek an element from the register's stack and push it onto the main stack
        auto value = this->regs[reg_name].stack.pop(false);
        this->stack.push(value);
    }

    return std::nullopt;
}

std::optional<std::string> Evaluate::parse_array_command(std::string token) {
    // An array command has length equal to 2, starts
    // with either ':'(store) or ';'(read) and ends with
    // the register name(i.e., ':X' or ';X')
    if(token.at(0) == ':') {
        // An ':' command pops two values from the main stack. The second-to-top
	    // element will be stored in the array indexed by the top-of-stack.
        auto reg_name = token.at(1);

        // Check if the main stack has enough elements
        if(this->stack.size() < 2) {
            return "This operation requires two values";
        }

        // Extract two elements from the main stack
        this->stack.copy_xyz();
        auto idx_str = this->stack.pop(true);
        auto arr_val = this->stack.pop(true);

        // Check whether the index is an integer
        if(!is_num<int>(idx_str)) {
            return "Array index must be an integer";
        }

        // Otherwise convert it into an integer
        auto idx = std::stoi(idx_str);

        // If array exists, store 'p' at index 'i' on array 'r'
        // If array does not exist, allocate a new array first
        auto it = this->regs.find(reg_name);
        if(it != this->regs.end()) { // Register exists
            // Always discard previous values of array
            it->second.array.erase(idx);
            it->second.array.insert(std::pair<int, std::string>(idx, arr_val));
        } else { // Register doesn't exist
            this->regs[reg_name] = Register{
                DCStack<std::string>(),
                std::unordered_map<int, std::string>{{idx, arr_val}}
            };
        }
    } else {
        // An ';' command pops top-of-stack abd uses it as an index
    	// for the array. The selected value, if any, is pushed onto the stack
        auto reg_name = token.at(1);

        // Check if the main stack is empty
        if(this->stack.empty()) {
            return "This operation requires one value";
        }

        // Extract the index from the stack
        this->stack.copy_xyz();
        auto idx_str = this->stack.pop(true);

        // Check if index is an integer
        if(!is_num<int>(idx_str)) {
            return "Array index must be an integer";
        }

        // Otherwise, convert it to integer
        auto idx = std::stoi(idx_str);

        // Check if the array exists
        if(this->regs.find(reg_name) == this->regs.end()) {
            return std::string("Register '") + reg_name + std::string("' is undefined");
        }

        // Check if array is empty
        if(this->regs[reg_name].array.empty()) {
            return std::string("The array of register '") + reg_name + std::string("' is empty");
        }

        // Otherwise, use the index to retrieve the array element
        // and to push it onto the main stack
        auto reg_it = regs.find(reg_name);
        auto arr_it = reg_it->second.array.find(idx);

        if(arr_it != reg_it->second.array.end()) {
            this->stack.push(arr_it->second);
        } else {
            return std::string("Cannot access ") + reg_name +
                   std::string("[") + std::to_string(idx) + std::string("]");
        }
    }

    return std::nullopt;
}
