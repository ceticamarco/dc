#include <numeric>
#include "adt.h"

#define GET_X this->stack.back()
#define GET_Y this->stack.at(this->stack.size() - 2)
#define GET_Z this->stack.at(this->stack.size() - 3)

namespace dc {
    /**
     * @brief Pushes a value onto the stack
     * @param value The value to be pushed
     */
    template<typename T>
    requires is_num_or_str<T>
    void Stack<T>::push(T value) {
        this->stack.push_back(value);
    }

    /**
     * @brief Empties the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    void Stack<T>::clear() {
        if(!this->stack.empty()) {
            this->stack.clear();
        }
    }

    /**
     * @brief Pops the head of the stack and returns it
     * if the _remove_ flag is true, otherwise returns the 
     * head of the stack without removing it(i.e. peek)
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
     * @brief Copies the head, the 2nd and the 3rd 
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
     * @brief Gets the _last x_(i.e., the previous head) value of the stack. If stack is empty,
     * returns '0' or empty string
     * @return _last x_ element of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::get_last_x() {
        return this->last_x;
    }

    /**
     * @brief Gets the _last y_(i.e., the previous second-to-top) value of the stack. If stack is empty,
     * returns '0' or empty string
     * @return _last y_ element of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::get_last_y() {
        return this->last_y;
    }

    /**
     * @brief Gets the _last z_(i.e., the previous third-to-top) value of the stack. If stack is empty,
     * returns '0' or empty string
     * @return _last z_ element of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T Stack<T>::get_last_z() {
        return this->last_z;
    }

    /**
     * @brief Gets the _nth_ element of the stack
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
     * @brief Overloads the '[]' operator to match the Stack::at function
     * @param index The index of the element to get
     * @return The _nth_ value of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    T& Stack<T>::operator[](std::size_t index) {
        return at(index);
    }

    /**
     * @brief Gets the size of the stack
     * @return The size of the stack
     */
    template<typename T>
    requires is_num_or_str<T>
    std::size_t Stack<T>::size() {
        return this->stack.size();
    }

    /**
     * @brief Gets the summation of all items on the stack
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
     * @brief Gets the sum of the squares of the stack
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
     * @brief Returns true if stack is empty false otherwise
     * @return Boolean value
     */
    template<typename T>
    requires is_num_or_str<T>
    bool Stack<T>::empty() {
        return this->stack.empty();
    }

    /**
    * @brief Gets a const reference to the stack
    * @return The const reference to the stack
    */
    template<typename T>
    requires is_num_or_str<T>
    const std::vector<T>& Stack<T>::get_ref() const {
        return this->stack;
    }
}
