#ifndef StackARRAY_TPP
#define StackARRAY_TPP

#include "ArrayStack.hpp"

template <typename T>
ArrayStack<T>::ArrayStack() = default;

template <typename T>
ArrayStack<T>::~ArrayStack() = default;

template <typename T>
ArrayStack<T>::ArrayStack(const ArrayStack& other) : elements(other.elements) {}

template <typename T>
ArrayStack<T>& ArrayStack<T>::operator=(const ArrayStack& other) {
    if (this != &other) {
        elements = other.elements;
    }

    return *this;
}

template <typename T>
ArrayStack<T>::ArrayStack(ArrayStack&& other) noexcept : elements(std::move(other.elements)) {}

template <typename T>
ArrayStack<T>& ArrayStack<T>::operator=(ArrayStack&& other) noexcept {
    if (this != &other) {
        elements = std::move(other.elements);
    }

    return *this;
}

template <typename T>
void ArrayStack<T>::push(ArrayStack<T>::const_reference element) {
    elements.push_back(element);
}

template <typename T>
void ArrayStack<T>::emplace(ArrayStack<T>::value_type&& element) {
    elements.emplace_back(std::move(element));
}

template <typename T>
ArrayStack<T>::const_reference ArrayStack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("ArrayStack is empty");
    }
    
    return elements.back();
}

template <typename T>
ArrayStack<T>::reference ArrayStack<T>::top() {
    if (empty()) {
        throw std::out_of_range("ArrayStack is empty");
    }
    
    return elements.back();
}

template <typename T>
bool ArrayStack<T>::empty() const {
    return elements.empty();
}

template <typename T>
typename ArrayStack<T>::value_type ArrayStack<T>::peek() const {
    if (empty()) {
        throw std::out_of_range("ArrayStack is empty");
    }
    
    return elements.back();
}

template <typename T>
void ArrayStack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("ArrayStack is empty");
    }
    
    elements.pop_back();
}

template <typename T>
typename ArrayStack<T>::size_type ArrayStack<T>::size() const {
    return elements.size();
}

#endif