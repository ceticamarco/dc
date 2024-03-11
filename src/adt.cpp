#include "adt.h"

/**
 * Add new element to the stack
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
 * Read stack at index
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