#pragma once
#include <vector>
#include <string>
#include <unordered_map>

using dc_stack_t = std::vector<std::string>;
typedef struct {
    dc_stack_t stack;
    std::unordered_map<int, std::string> array;
} Register;
