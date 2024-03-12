#include "adt.h"

#define GET_X this->stack.back()
#define GET_Y this->stack.at(this->stack.size() - 2)
#define GET_Z this->stack.at(this->stack.size() - 3)

/**
 * Add @value to the stack
 */
template<typename T>
requires is_num_or_str<T>
void DCStack<T>::push(T value) {
    this->stack.push_back(value);
}

/**
 * Clear the stack
 */
template<typename T>
requires is_num_or_str<T>
void DCStack<T>::clear() {
    if(!this->stack.empty()) {
        this->stack.clear();
    }
}

/**
 * If @remove is true, pop head of the stack
 * otherwise return it without popping.
 * If stack is empty it causes undefined behavior
 */
template<typename T>
requires is_num_or_str<T>
T DCStack<T>::pop(bool remove) {
    T value = this->stack.back();

    if(remove) {
        this->stack.pop_back();
    }

    return value;
}

/**
 * Make a copy of head, 2nd and 3rd element
 * of the stack
 */
template<typename T>
requires is_num_or_str<T>
void DCStack<T>::copy_xyz() {
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
 * Returns last x of the stack
 * If stack is empty returns '0' or empty string
 */
template<typename T>
requires is_num_or_str<T>
T DCStack<T>::get_last_x() {
    return this->last_x;
}

/**
 * Returns last y of the stack
 * If stack is empty returns '0' or empty string
 */
template<typename T>
requires is_num_or_str<T>
T DCStack<T>::get_last_y() {
    return this->last_y;
}

/**
 * Returns last z of the stack
 * If stack is empty returns '0' or empty string
 */
template<typename T>
requires is_num_or_str<T>
T DCStack<T>::get_last_z() {
    return this->last_z;
}

/**
 * Reads stack at @index
 */
template<typename T>
requires is_num_or_str<T>
T& DCStack<T>::at(size_t index) {
    T &value = this->stack.at(index);

    return value;
}

/**
 * Returns stack size
 */
template<typename T>
requires is_num_or_str<T>
size_t DCStack<T>::size() {
    return this->stack.size();
}

/**
 * Returns true if stack is empty
 * false otherwise
 */
template<typename T>
requires is_num_or_str<T>
bool DCStack<T>::empty() {
    return this->stack.empty();
}

/**
* Returns a const reference to the stack
*/
template<typename T>
requires is_num_or_str<T>
const std::vector<T>& DCStack<T>::get_ref() const {
    return this->stack;
}
