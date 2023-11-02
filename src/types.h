#pragma once
#include <vector>
#include <string>
#include <unordered_map>

using stack_t = std::vector<std::string>;
typedef struct {
    stack_t stack;
    std::unordered_map<int, std::string> array;
} Register;
