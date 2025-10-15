#include "ArrayDeque.h"

template <typename T>
ArrayDeque<T>::ArrayDeque(size_t initial_capacity)
    : array(initial_capacity),
      count(0),
      front_idx(0),
      back_idx(initial_capacity - 1) {}

template <typename T>
void ArrayDeque<T>::reserve_if_needed() {
    if (count == array.capacity()) {
        size_t new_capacity = array.capacity() * 2;
        std::vector<T> new_array(new_capacity);
        
        for (size_t i = 0; i < count; i++) {
            new_array[i] = array[(front_idx + i) % array.capacity()];
        }
        
        array = std::move(new_array);
        front_idx = 0;
        back_idx = count - 1;
    }
}

template <typename T>
void ArrayDeque<T>::push_front(const T& value) {
    reserve_if_needed();
    
    front_idx = (front_idx - 1 + array.capacity()) % array.capacity();
    array[front_idx] = value;
    count++;
}

template <typename T>
void ArrayDeque<T>::push_back(const T& value) {
    reserve_if_needed();
    
    back_idx = (back_idx + 1) % array.capacity();
    array[back_idx] = value;
    count++;
}

template <typename T>
void ArrayDeque<T>::pop_front() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    front_idx = (front_idx + 1) % array.capacity();
    count--;
}

template <typename T>
void ArrayDeque<T>::pop_back() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    back_idx = (back_idx - 1 + array.capacity()) % array.capacity();
    count--;
}

template <typename T>
T& ArrayDeque<T>::front() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    return array[front_idx];
}

template <typename T>
T& ArrayDeque<T>::back() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    return array[back_idx];
}

template <typename T>
bool ArrayDeque<T>::empty() const {
    return count == 0;
}

template <typename T>
size_t ArrayDeque<T>::size() const {
    return count;
}