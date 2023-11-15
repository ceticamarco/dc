#include <memory>

#include "eval.h"
#include "math.h"
#include "stack.h"
#include "macro.h"
#include "is_num.h"

#define MACRO_COND(VAL) ((VAL.length() == 1 && VAL == "["))
#define MACRO_CMD_COND(VAL) ((val.length() == 2 || val.length() == 3) && \
              (val.at(0) == '>' || val.at(0) == '<' || \
               val.at(0) == '=' || val.at(0) == '!'))
#define REGISTER_COND(VAL) ((val.length() == 2) && \
              (val.at(0) == 's' || val.at(0) == 'S' || \
               val.at(0) == 'l' || val.at(0) == 'L'))
#define ARRAY_COND(VAL) ((val.length() == 2) && \
        (val.at(0) == ':' || val.at(0) == ';'))

std::optional<std::string> Evaluate::eval() {
    for(size_t idx = 0; idx < this->expr.size(); idx++) {
        auto val = this->expr.at(idx);
        std::optional<std::string> err = std::nullopt;
    
        //
        // 		NUMERICAL OPERATIONS
        //
        if(val == "+") {
            auto math = std::make_unique<Math>(OPType::ADD, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "-") {
            auto math = std::make_unique<Math>(OPType::SUB, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "*") {
            auto math = std::make_unique<Math>(OPType::MUL, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val =="/") {
            auto math = std::make_unique<Math>(OPType::DIV, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "%") {
            auto math = std::make_unique<Math>(OPType::MOD, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "~") {
            auto math = std::make_unique<Math>(OPType::DIV_MOD, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "|") {
            auto math = std::make_unique<Math>(OPType::MOD_EXP, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "^") {
            auto math = std::make_unique<Math>(OPType::EXP, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "v") {
            auto math = std::make_unique<Math>(OPType::SQRT, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "sin") {
            auto math = std::make_unique<Math>(OPType::SIN, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "cos") {
            auto math = std::make_unique<Math>(OPType::COS, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "tan") {
            auto math = std::make_unique<Math>(OPType::TAN, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "!") {
            auto math = std::make_unique<Math>(OPType::FACT, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "pi") {
            auto math = std::make_unique<Math>(OPType::PI, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "e") {
            auto math = std::make_unique<Math>(OPType::E, this->parameters.precision);
            err = math->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        }
        //
        // 		STACK OPERATIONS
        //
        else if(val == "p") { // PRINT TOP ELEMENT OF STACK
            auto op = std::make_unique<Stack>(OPType::PCG);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "pb") { // PRINT TOP ELEMENT IN BASE 2
            auto op = std::make_unique<Stack>(OPType::PBB);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "ph") { // PRINT TOP ELEMENT IN BASE 16
            auto op = std::make_unique<Stack>(OPType::PBH);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "po") { // PRINT TOP ELEMENT IN BASE 8
            auto op = std::make_unique<Stack>(OPType::PBO);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "P") { // PRINT TOP ELEMENT WITHOUT NEWLINE
            auto op = std::make_unique<Stack>(OPType::P);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "c") { // CLEAR THE STACK
            auto op = std::make_unique<Stack>(OPType::CLR);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "R") { // POP HEAD OF THE STACK WITHOUT PRINTING IT
            auto op = std::make_unique<Stack>(OPType::PH);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "r") { // SWAP ORDER OF THE TWO TOP ELEMENTS
            auto op = std::make_unique<Stack>(OPType::SO);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "d") { // DUPLICATE THE HEAD OF THE STACK
            auto op = std::make_unique<Stack>(OPType::DP);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "f") { // PRINT THE WHOLE STACK
            auto op = std::make_unique<Stack>(OPType::PS);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "Z") { // COMPUTE HEAD SIZE(NUM. OF CHARS/DIGITS)
            auto op = std::make_unique<Stack>(OPType::CH);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "z") { // COMPUTE STACK SIZE
            auto op = std::make_unique<Stack>(OPType::CS);
            err = op->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "k") { // SET PRECISION
            err = fn_set_precision();
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "K") { // GET PRECISION
            err = fn_get_precision();
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "o") { // SET OUTPUT BASE
        } else if(val == "O") { // GET OUTPUT BASE
        } else if(val == "i") { // SET INPUT BASE
        } else if(val == "I") { // GET INPUT BASE
        } else if(val == "x") { // EXECUTE MACRO
            auto macro = std::make_unique<Macro>(OPType::EX, this->regs, this->parameters);
            err = macro->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "?") { // READ LINE FROM STDIN
            auto macro = std::make_unique<Macro>(OPType::RI, this->regs, this->parameters);
            err = macro->exec(this->stack);
            if(err != std::nullopt) {
                return err;
            }
        } else if(val == "q") { // QUIT GRACEFULLY
            std::exit(0);
        } else {
            err = handle_special(val, idx);
            if(err != std::nullopt) {
                return err;
            }
        }
    }

    return std::nullopt;
}

std::optional<std::string> Evaluate::handle_special(std::string val, size_t &idx) {
    std::optional<std::string> err = std::nullopt;

    if(MACRO_COND(val)) {
        err = parse_macro(idx);
    } else if(MACRO_CMD_COND(val)) {
        err = parse_macro_command(val);
    } else if(REGISTER_COND(val)) {
        err = parse_register_command(val);
    } else if(ARRAY_COND(val)) {
        err = parse_array_command(val);
    } else if(is_num<double>(val)) {
        this->stack.push_back(val);
    } else {
        return "Unrecognized option";
    }

    return err;
}

std::optional<std::string> Evaluate::parse_macro(size_t &idx) {
    // A macro is any string surrounded by square brackets
    std::string dc_macro = "";
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
    this->stack.push_back(dc_macro);

    return std::nullopt;
}

std::optional<std::string> Evaluate::parse_macro_command(std::string val) {
    // A macro command is a comparison symbol(>, <, =, >=, <=, !=)
    // followed by a register name(e.g, >A)
    // If command has length equal to three, then it's either '<=', '>=' or '!='

    // Check if command is >=, <= or !=
    std::string operation = "";
    char dc_register = 0;
    if(val.length() == 3) {
        operation = val.substr(0, 2);
        dc_register = val.at(2);
    } else { // Otherwise it's either >, < or =
        operation = val.at(0);
        dc_register = val.at(1);
    }

    // Macro commands works as follow
    // Pop two values off the stack and compares them assuming
    // they are numbers. If top-of-stack is greater,
    // execute register's content as a macro
    std::optional<std::string> err = std::nullopt;
    if(operation == ">") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::GT, dc_register, this->regs, this->parameters);
        err = macro->exec(this->stack);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "<") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::LT, dc_register, this->regs, this->parameters);
        err = macro->exec(this->stack);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::EQ, dc_register, this->regs, this->parameters);
        err = macro->exec(this->stack);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == ">=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::GEQ, dc_register, this->regs, this->parameters);
        err = macro->exec(this->stack);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "<=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::LEQ, dc_register, this->regs, this->parameters);
        err = macro->exec(this->stack);
        if(err != std::nullopt) {
            return err;
        }
    } else if(operation == "!=") {
        auto macro = std::make_unique<Macro>(OPType::CMP, Operator::NEQ, dc_register, this->regs, this->parameters);
        err = macro->exec(this->stack);
        if(err != std::nullopt) {
            return err;
        }
    }

    return err;
}

std::optional<std::string> Evaluate::parse_register_command(std::string val) {
    // A register command has length equal to 2
    // and starts either with 's', 'l'(i.e. "sX" or "lX")
    // or with 'S' or 'L'(i.e., "SX", "LX")
    if(val.at(0) == 's') {
        // Check if main stack is empty
        if(this->stack.empty()) {
            return "This operation does not work on empty stack";
        }

        // Otherwise pop an element from main stack and store it into
        // the register's top-of-the-stack. Any previous value gets overwritten
        auto reg_name = val.at(1);
        auto head = this->stack.back();
        this->stack.pop_back();

        // Always discard previous instance of the register
        // i.e., initialize a new instance of register 'reg_name'
        this->regs.insert(
                std::make_pair(reg_name, Register{
                    std::vector<std::string>(),
                    std::unordered_map<int, std::string>()
                })
        );

         // Populate register's 'reg_name' stack with top of main stack
         this->regs[reg_name].stack.push_back(head);
    } else if(val.at(0) == 'S') {
        // An uppercase 'S' pops the top of the main stack and
	// pushes it onto the stack of selected register.
	// The previous value of the register's stack becomes
	// inaccessible(i.e. it follows LIFO policy).

        // Check if main stack is empty
        if(this->stack.empty()) {
            return "This operation does not work on empty stack";
        }

        auto reg_name = val.at(1);
        auto head = this->stack.back();
        this->stack.pop_back();

        // If register's stack exist, push an element onto its stack
        // otherwise allocate a new instance of the register
        auto it = this->regs.find(reg_name);
        if(it != this->regs.end()) { // Register exists
            it->second.stack.push_back(head);
        } else { // Register doesn't exist
            this->regs[reg_name] = Register{
                std::vector<std::string>{head},
                std::unordered_map<int, std::string>()
            };
        }
    } else if(val.at(0) == 'L') {
        // An uppercase 'L' pops the top of the register's stack
	// abd pushes it onto the main stack. The previous register's stack
	// value, if any, is accessible via the lowercase 'l' command
        auto reg_name = val.at(1);

        // Check if register exists
        if(this->regs.find(reg_name) == this->regs.end()) {
            return std::string("Register '") + reg_name + std::string("' is undefined");
        }

        // Check if register's stack is empty
        if(this->regs[reg_name].stack.empty()) {
            return std::string("The stack of register '") + reg_name + std::string(" is empty");
        }

        // Otherwise, pop an element from the register's stack and push it onto the main stack
        auto value = this->regs[reg_name].stack.back();
        this->regs[reg_name].stack.pop_back();
        this->stack.push_back(value);
    } else {
        // Otherwise retrieve the register name and push its value
	// to the stack without altering the register's stack.
	// If the register is empty, push '0' to the stack
        auto reg_name = val.at(1);

        // If register does not exists or its stack is empty, push '0' onto the main stack
        auto it = this->regs.find(reg_name);
        if(it == this->regs.end() || it->second.stack.empty()) {
            this->stack.push_back("0");
            return std::nullopt;
        }

        // Otherwise, peek an element from the register's stack and push it onto the main stack
        auto value = this->regs[reg_name].stack.back();
        this->stack.push_back(value);
    }

    return std::nullopt;
}

std::optional<std::string> Evaluate::parse_array_command(std::string val) {
    // An array command has length equal to 2, starts
    // with either ':'(store) or ';'(read) and ends with
    // the register name(i.e., ':X' or ';X')
    if(val.at(0) == ':') {
        // An ':' command pops two values from the main stack. The second-to-top
	// element will be stored in the array indexed by the top-of-stack.
        auto reg_name = val.at(1);

        // Check if the main stack has enough elements
        if(this->stack.size() < 2) {
            return "This operation requires two values";
        }

        // Extract two elements from the main stack
        auto idx_str = this->stack.back();
        this->stack.pop_back();
        auto arr_val = this->stack.back();
        this->stack.pop_back();

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
            it->second.array.insert(std::pair<int, std::string>(idx, arr_val));
        } else { // Register doesn't exist
            this->regs[reg_name] = Register{
                std::vector<std::string>(),
                std::unordered_map<int, std::string>{{idx, arr_val}}
            };
        }
    } else {
        // An ';' command pops top-of-stack abd uses it as an index
	// for the array. The selected value, if any, is pushed onto the stack
        auto reg_name = val.at(1);

        // Check if the main stack is empty
        if(this->stack.empty()) {
            return "This operation requires one value";
        }

        // Extract the index from the stack
        auto idx_str = this->stack.back();
        this->stack.pop_back();

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
            this->stack.push_back(arr_it->second);
        } else {
            return std::string("Cannot access ") + reg_name +
                   std::string("[") + std::to_string(idx) + std::string("]");
        }
    }

    return std::nullopt;
}

std::optional<std::string> Evaluate::fn_set_precision() {
    // Check if stack has enough elements
    if(this->stack.empty()) {
        return "'k' requires one operand";
    }

    // Pop the head of the stack
    auto head = std::stol(this->stack.back());
    this->stack.pop_back();

    // Check if precision is non-negative
    if(head < 0) {
        return "Precision must be >=0";
    }

    // Set precision parameter
    this->parameters.precision = head;

    return std::nullopt;
}

std::optional<std::string> Evaluate::fn_get_precision() {
    this->stack.push_back(std::to_string(this->parameters.precision));

    return std::nullopt;
}

std::optional<std::string> Evaluate::fn_set_oradix() {
    return std::nullopt;
}

std::optional<std::string> Evaluate::fn_get_oradix() {
    return std::nullopt;
}

std::optional<std::string> Evaluate::fn_set_iradix() {
    return std::nullopt;
}

std::optional<std::string> Evaluate::fn_get_iradix() {
    return std::nullopt;
}
