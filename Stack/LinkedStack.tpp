#ifndef LinkedStackLIST_TPP
#define LinkedStackLIST_TPP

#include "LinkedStack.hpp"

template <typename T>
LinkedStack<T>::LinkedStack() = default;

template <typename T>
LinkedStack<T>::~LinkedStack() = default;

template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack& other) : elements_(other.elements_) {}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(const LinkedStack& other) {
    if (this != &other) {
        elements_ = other.elements_;
    }

    return *this;
}

template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack&& other) noexcept : elements_(std::move(other.elements_)) {}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack&& other) noexcept {
    if (this != &other) {
        elements_ = std::move(other.elements_);
    }
    
    return *this;
}

template <typename T>
void LinkedStack<T>::push(LinkedStack<T>::const_reference element) {
    elements_.push_front(element);
}

template <typename T>
void LinkedStack<T>::emplace(LinkedStack<T>::value_type&& element) {
    elements_.emplace_front(std::move(element));
}

template <typename T>
LinkedStack<T>::const_reference LinkedStack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("LinkedStack is empty");
    }
    
    return elements_.front();
}

template <typename T>
LinkedStack<T>::reference LinkedStack<T>::top() {
    if (empty()) {
        throw std::out_of_range("LinkedStack is empty");
    }
    
    return elements_.front();
}

template <typename T>
bool LinkedStack<T>::empty() const {
    return elements_.empty();
}

template <typename T>
typename LinkedStack<T>::value_type LinkedStack<T>::peek() const {
    if (empty()) {
        throw std::out_of_range("LinkedStack is empty");
    }
    
    return elements_.front();
}

template <typename T>
void LinkedStack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("LinkedStack is empty");
    }
    
    elements_.pop_front();
}

template <typename T>
typename LinkedStack<T>::size_type LinkedStack<T>::size() const {
    return elements_.size();
}

#endif