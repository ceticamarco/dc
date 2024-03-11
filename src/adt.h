#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

template<typename T>
concept is_num_or_str = (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>);

template<typename T>
requires is_num_or_str<T>
class DCStack {
public:
    DCStack() = default;
    void push(T value);
    void clear();
    T pop(bool remove);
    T& at(size_t index);
    size_t size();
    bool empty();
    [[nodiscard]] const std::vector<T>& get_ref() const;

private:
    std::vector<T> stack;
    T last_x;
    T last_y;
    T last_z;
};

typedef struct {
    DCStack<std::string> stack;
    std::unordered_map<int, std::string> array;
} Register;

enum class radix_base : std::uint8_t { BIN = 2, OCT = 8, DEC = 10, HEX = 16 };
typedef struct {
    unsigned int precision;
    unsigned short iradix;
    radix_base oradix;
} Parameters;
