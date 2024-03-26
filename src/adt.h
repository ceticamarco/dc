#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

namespace dc {
    template<typename T>
    concept is_num_or_str = (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>);

    template<typename T>
    requires is_num_or_str<T>
    class Stack {
    public:
        void push(T value);
        void clear();
        void copy_xyz();
        T pop(bool remove);
        T get_last_x();
        T get_last_y();
        T get_last_z();
        T& at(std::size_t index);
        T& operator[](std::size_t index);
        std::size_t size();
        double summation();
        double summation_squared();
        bool empty();
        [[nodiscard]] const std::vector<T>& get_ref() const;

    private:
        std::vector<T> stack;
        T last_x{};
        T last_y{};
        T last_z{};
    };

    typedef struct {
        Stack<std::string> stack;
        std::unordered_map<int, std::string> array;
    } Register;

    enum class radix_base : std::uint8_t { BIN = 2, OCT = 8, DEC = 10, HEX = 16 };
    typedef struct {
        unsigned int precision;
        unsigned short iradix;
        radix_base oradix;
    } Parameters;
}
