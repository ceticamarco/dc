#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

using dc_stack_t = std::vector<std::string>;
typedef struct {
    dc_stack_t stack;
    std::unordered_map<int, std::string> array;
} Register;

enum class radix_base : std::uint8_t { BIN = 2, OCT = 8, DEC = 10, HEX = 16 };
typedef struct {
    unsigned int precision;
    unsigned short iradix;
    radix_base oradix;
} Parameters;
