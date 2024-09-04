#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>

namespace dc {
    /**
     * @brief Constrains a generic type to either integral/float types or to a string
     */
    template<typename T>
    concept is_num_or_str = (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>);

    /**
     * @brief Stack abstract data type
     * 
     * Wraps the C++ std::vector container to create a stack-like data structure
     */
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
    
    /**
     * @brief Register data structure
     * 
     * This abstract data type is made of an isolated stack, represented by the Stack data type, 
     * and an array represented by an hashmap
     */
    typedef struct {
        Stack<std::string> stack;
        std::unordered_map<int, std::string> array;
    } Register;

    enum class radix_base : std::uint8_t { BIN = 2, OCT = 8, DEC = 10, HEX = 16 };
    /**
     * @brief Parameters data structure
     * 
     * This abstract data type groups together the precision of numeric operations, the input radix
     * and the output radix
     */
    typedef struct {
        unsigned int precision;
        unsigned short iradix;
        radix_base oradix;
    } Parameters;
}
