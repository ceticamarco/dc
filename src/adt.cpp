#include <numeric>
#include "adt.h"

#define GET_X this->stack.back()
#define GET_Y this->stack.at(this->stack.size() - 2)
#define GET_Z this->stack.at(this->stack.size() - 3)

namespace dc {
    /**
     * @brief push a value onto the stack
     * @param value The value to be pushed
     */
    template<typename T>
    requires is_num_or_str<T>
    void Stack<T>::push(T value) {
        this->stack.push_back(value);
    }

    /**
     * @brief Empty the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    void Stack<T>::clear() {
        if(!this->stack.empty()) {
            this->stack.clear();
        }
    }

    /**
     * @brief Pop the head of the stack and return it
     * if the _remove_ flag is true, otherwise return the 
     * head of the stack without removing it(i.e. peek it)
     * 
     * @param remove Specify whether the operation is disruptive or not
     * @return The head of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::pop(bool remove) {
        T value = this->stack.back();

        if(remove) {
            this->stack.pop_back();
        }

        return value;
    }

    /**
     * @brief Make a copy of the head, the 2nd and the 3rd 
     * elements of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    void Stack<T>::copy_xyz() {
        if(this->stack.size() >= 1) {
            this->last_x = GET_X;
        }

        if(this->stack.size() >= 2) {
            this->last_y = GET_Y;
        }

        if(this->stack.size() >= 3) {
            this->last_z = GET_Z;
        }
    }

    /**
     * @brief Get the _last x_(i.e., the previous head) value of the stack. If stack is empty,
     * return '0' or empty string
     * @return _last x_ element of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::get_last_x() {
        return this->last_x;
    }

    /**
     * @brief Get the _last y_(i.e., the previous second-to-top) value of the stack. If stack is empty,
     * return '0' or empty string
     * @return _last y_ element of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::get_last_y() {
        return this->last_y;
    }

    /**
     * @brief Get the _last z_(i.e., the previous third-to-top) value of the stack. If stack is empty,
     * return '0' or empty string
     * @return _last z_ element of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::get_last_z() {
        return this->last_z;
    }

    /**
     * @brief Get the _nth_ element of the stack
     * @param index The index of the element to get
     * @return The _nth_ value of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T& Stack<T>::at(std::size_t index) {
        T &value = this->stack.at(index);

        return value;
    }

    /**
     * @brief Overload the '[]' operator to match the 'at' function
     * @param index The index of the element to get
     * @return The _nth_ value of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T& Stack<T>::operator[](std::size_t index) {
        return at(index);
    }

    /**
     * @brief Get the current value of the stack
     * @return The value of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    std::size_t Stack<T>::size() {
        return this->stack.size();
    }

    /**
     * @brief Get the summation of all items on the stack
     * @return The sum of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    double Stack<T>::summation() {
        auto sum = std::accumulate(this->stack.begin(), this->stack.end(), 0.0,
            [](auto accumulator, const T& val) -> double {
                if constexpr(std::is_same_v<T, std::string>) {
                    return accumulator + std::stod(val);
                } else {
                    return accumulator + val;
                }
            });

        return sum;
    }

    /**
     * @brief Get the summation of squares of all items
     * @return The summation of squares of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    double Stack<T>::summation_squared() {
        auto sum = std::accumulate(this->stack.begin(), this->stack.end(), 0.0,
            [](auto accumulator, const T& val) -> double {
                if constexpr(std::is_same_v<T, std::string>) {
                    return accumulator + (std::stod(val) * std::stod(val));
                } else {
                    return accumulator + (val * val);
                }
            });

        return sum;
    }


    /**
     * @brief Return true if stack is empty false otherwise
     * @return Boolean value
     */
    template<typename T>
    requires is_num_or_str<T>
    bool Stack<T>::empty() {
        return this->stack.empty();
    }

    /**
    * @brief Get a const reference to the stack
    * @return The const reference to the stack
    */
    template<typename T>
    requires is_num_or_str<T>
    const std::vector<T>& Stack<T>::get_ref() const {
        return this->stack;
    }
}
