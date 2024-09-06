#include <iostream>
#include <getopt.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>

#include "src/adt.h"
#include "src/eval.h"
#include "src/macro.h" // for split static method

using namespace dc;

/**
 * @brief helper function of the program
 */

void helper() {
    std::cout << "RPN desktop calculator and stack-based programming language. Usage:\n"
              << "-e, --expression <EXPRESSION> | Evaluate an expression\n"
              << "-f, --file <FILE>             | Evaluate a file\n"
              << "-h, --help                    | Show this helper\n"
              << "-V, --version                 | Show version" << std::endl;
}

/**
 * @brief Prints various information about the program
 */

void version() {
    std::cout << "dc (v" << DC_VER << ", " << DC_HASH << ", " << DC_BUILD ")" << std::endl;
    std::cout << "Build date: " << DC_BUILD_DATE << std::endl;
    std::cout << "Compiled with: " << DC_COMPILER << " v" << DC_COMPILER_V << std::endl;
    std::cout << "Compiler flags: " << DC_FLAGS << std::endl;
    std::cout << "Copyright (c) 2024 Marco Cetica" << std::endl;
    std::cout << "License GPLv3+: GNU GPL version 3 or later\n" << std::endl;
    std::cout << "The original version of the dc command was written by Robert Morris\n"
              << "and Lorinda Cherry. This version of dc is developed by Marco Cetica.\n" << std::endl;
    std::cout << "Project homepage: <https://git.marcocetica.com/marco/dc>.\n"
              << "Email bug reports to: <email@marcocetica.com>." << std::endl;
}

int main(int argc, char **argv) {
    int opt;
    const char *short_opts = "e:f:hV";
    std::string cli_expression;
    std::string file_name;
    std::string stdin_expression;
    bool execute_expression = false;
    bool execute_file = false;
    Stack<std::string> stack;
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
                version();
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
                return 1;
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
