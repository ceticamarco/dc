#include <iostream>
#include <getopt.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>

#include "src/types.h"
#include "src/eval.h"
#include "src/macro.h" // for split static method

#define DC_VERSION "1.0.1"

void helper() {
    std::cout << "RPN desktop calculator with macro support. Usage: \n"
              << "-e, --expression <EXPRESSION> | Evaluate an expression\n"
              << "-f, --file <FILE>             | Evaluate a file\n"
              << "-h, --help                    | Show this helper\n"
              << "-V, --version                 | Show version" << std::endl;
}

int main(int argc, char **argv) {
    int opt;
    const char *short_opts = "e:f:hV";
    std::string cli_expression = "";
    std::string file_name = "";
    std::string stdin_expression = "";
    bool execute_expression = false;
    bool execute_file = false;
    dc_stack_t stack;
    std::unordered_map<char, Register> regs;
    Parameters parameters = {
        .precision = 0,
        .iradix = 10,
        .oradix = radix_base::DEC
    };
    struct option long_opts[] = {
        {"expression", required_argument, nullptr, 'e'},
        {"file", required_argument, nullptr, 'f'},
        {"help", no_argument, nullptr, 'h'},
        {"version", no_argument, nullptr, 'V'},
        {nullptr, 0, nullptr, 0}
    };

    while((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch(opt) {
            case 'e': {
                cli_expression = std::string(optarg);
                execute_expression = true;
            }
            break;
            case 'f': {
                file_name = std::string(optarg);
                execute_file = true;
            }
            break;
            case 'V': {
                std::cout << "dc v" << DC_VERSION << std::endl;
                return 0;
            }
            break;
            default: case 'h': case ':': case '?': helper(); return 0;;
        }
    }

    // Evaluate cli expression
    if(execute_expression) {
        // Split string expression into a vector
        std::vector<std::string> tokens = Macro::split(cli_expression);
        // Evaluate expression
        Evaluate evaluator(tokens, regs, stack, parameters);
        auto err = evaluator.eval();
        // Handle errors
        if(err != std::nullopt) {
            std::cerr << err.value() << std::endl;
            return 1;
        }

        return 0;
    } else if(execute_file) {
        // Open file from disk
        std::fstream source_file(file_name, std::ios::in | std::ios::binary);
        if(source_file.fail()) {
            std::cerr << "Cannot open source file \"" << file_name << "\"." << std::endl;
            return 1;
        }

        // Read whole file into a buffer
        std::stringstream buf;
        buf << source_file.rdbuf();

        // Execute file line by line
        std::string line;
        while(std::getline(buf, line, '\n')) {
            // Ignore comments or empty lines
            if(line.empty() || line.starts_with('#')) {
                continue;
            }

            // Remove inline comments
            auto comment_pos = line.find('#');
            std::vector<std::string> tokens;

            if(comment_pos != std::string::npos) {
                // Convert only the first part of the line
                tokens = Macro::split(line.substr(0, comment_pos));
            } else {
                // Otherwise, convert the whole string
                tokens = Macro::split(line);
            }

            // Evaluate expression
            Evaluate evaluator(tokens, regs, stack, parameters);
            auto err = evaluator.eval();
            // Handle errors
            if(err != std::nullopt) {
                std::cerr << err.value() << std::endl;
            }
        }

        return 0;
    }

    
    // Otherwise, evaluate from stdin
    while(std::getline(std::cin, stdin_expression)) {
        // Split string expression into a vector
        std::vector<std::string> tokens = Macro::split(stdin_expression);
        // Evaluate expression
        Evaluate evaluator(tokens, regs, stack, parameters);
        auto err = evaluator.eval();
        // Handle errors
        if(err != std::nullopt) {
            std::cerr << err.value() << std::endl;
        }
    }

    return 0;
}
