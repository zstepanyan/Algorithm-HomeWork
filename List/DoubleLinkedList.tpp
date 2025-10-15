#ifndef DOUBLE_LINKED_LIST_TPP
#define DOUBLE_LINKED_LIST_TPP

#include "DoubleLinkedList.hpp"

template <typename T>
DoubleLinkedList<T>::Node::Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}

template <typename T>
DoubleLinkedList<T>::Node::Node(T&& val) : data(std::move(val)), prev(nullptr), next(nullptr) {}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList() : head(nullptr), tail(nullptr), sz(0) {}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(const DoubleLinkedList& other) : head(nullptr), tail(nullptr), sz(0) {
    copy_from(other);
}

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList(DoubleLinkedList&& other) noexcept
    : head(other.head), tail(other.tail), sz(other.sz) {
    other.head = nullptr;
    other.tail = nullptr;
    other.sz = 0;
}

template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList() {
    clear();
}

template <typename T>
void DoubleLinkedList<T>::copy_from(const DoubleLinkedList& other) {
    clear();
    Node* current = other.head;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(const DoubleLinkedList& other) {
    if (this != &other) {
        copy_from(other);
    }

    return *this;
}

template <typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator=(DoubleLinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        sz = other.sz;
        other.head = nullptr;
        other.tail = nullptr;
        other.sz = 0;
    }
    
    return *this;
}

template <typename T>
void DoubleLinkedList<T>::push_back(const T& value) {
    Node* new_node = new Node(value);
    if (empty()) {
        head = tail = new_node;
    } 
    else {
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }
    
    ++sz;
}

template <typename T>
void DoubleLinkedList<T>::push_front(const T& value) {
    Node* new_node = new Node(value);
    if (empty()) {
        head = tail = new_node;
    } 
    else {
        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    }
    
    ++sz;
}

template <typename T>
void DoubleLinkedList<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    Node* to_delete = tail;
    if (head == tail) {
        head = tail = nullptr;
    } 
    else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    
    delete to_delete;
    --sz;
}

template <typename T>
void DoubleLinkedList<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    Node* to_delete = head;
    if (head == tail) {
        head = tail = nullptr;
    } 
    else {
        head = head->next;
        head->prev = nullptr;
    }
    
    delete to_delete;
    --sz;
}

template <typename T>
T& DoubleLinkedList<T>::front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return head->data;
}

template <typename T>
const T& DoubleLinkedList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return head->data;
}

template <typename T>
T& DoubleLinkedList<T>::back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return tail->data;
}

template <typename T>
const T& DoubleLinkedList<T>::back() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return tail->data;
}

template <typename T>
size_t DoubleLinkedList<T>::size() const {
    return sz;
}

template <typename T>
bool DoubleLinkedList<T>::empty() const {
    return sz == 0;
}

template <typename T>
void DoubleLinkedList<T>::clear() {
    while (!empty()) {
        pop_front();
    }
}

#endif