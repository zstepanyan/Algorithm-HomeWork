#ifndef ARRAY_LIST_TPP
#define ARRAY_LIST_TPP

#include "ArrayList.hpp"

template <typename T>
ArrayList<T>::ArrayList() : arr(nullptr), capacity(0), sz(0) {}

template <typename T>
ArrayList<T>::ArrayList(const ArrayList& other) : arr(new T[other.capacity]), capacity(other.capacity), sz(other.sz) {
    for (size_t i = 0; i < sz; ++i) {
        arr[i] = other.arr[i];
    }
}

template <typename T>
ArrayList<T>::ArrayList(ArrayList&& other) noexcept
    : arr(other.arr), capacity(other.capacity), sz(other.sz) {
    other.arr = nullptr;
    other.capacity = 0;
    other.sz = 0;
}

template <typename T>
ArrayList<T>::~ArrayList() {
    delete[] arr;
}

template <typename T>
void ArrayList<T>::resize(size_t new_capacity) {
    T* new_arr = new T[new_capacity];
    for (size_t i = 0; i < sz; ++i) {
        new_arr[i] = std::move(arr[i]);
    }

    delete[] arr;
    arr = new_arr;
    capacity = new_capacity;
}

template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other) {
    if (this != &other) {
        delete[] arr;
        arr = new T[other.capacity];
        capacity = other.capacity;
        sz = other.sz;
        for (size_t i = 0; i < sz; ++i) {
            arr[i] = other.arr[i];
        }
    }
    
    return *this;
}

template <typename T>
ArrayList<T>& ArrayList<T>::operator=(ArrayList&& other) noexcept {
    if (this != &other) {
        delete[] arr;
        arr = other.arr;
        capacity = other.capacity;
        sz = other.sz;
        other.arr = nullptr;
        other.capacity = 0;
        other.sz = 0;
    }
    
    return *this;
}

template <typename T>
void ArrayList<T>::push_back(const T& value) {
    if (sz == capacity) {
        size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
        resize(new_capacity);
    }
    
    arr[sz++] = value;
}

template <typename T>
void ArrayList<T>::push_front(const T& value) {
    if (sz == capacity) {
        size_t new_capacity = capacity == 0 ? 1 : capacity * 2;
        resize(new_capacity);
    }
    
    for (size_t i = sz; i > 0; --i) {
        arr[i] = std::move(arr[i - 1]);
    }
    
    arr[0] = value;
    ++sz;
}

template <typename T>
void ArrayList<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    --sz;
}

template <typename T>
void ArrayList<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    for (size_t i = 1; i < sz; ++i) {
        arr[i - 1] = std::move(arr[i]);
    }
    
    --sz;
}

template <typename T>
T& ArrayList<T>::front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return arr[0];
}

template <typename T>
const T& ArrayList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return arr[0];
}

template <typename T>
T& ArrayList<T>::back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return arr[sz - 1];
}

template <typename T>
const T& ArrayList<T>::back() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return arr[sz - 1];
}

template <typename T>
size_t ArrayList<T>::size() const {
    return sz;
}

template <typename T>
bool ArrayList<T>::empty() const {
    return sz == 0;
}

template <typename T>
void ArrayList<T>::clear() {
    sz = 0;
}

#endif