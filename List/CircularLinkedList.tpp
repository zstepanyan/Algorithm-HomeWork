#ifndef CIRCULAR_LINKED_LIST_TPP
#define CIRCULAR_LINKED_LIST_TPP

#include "CircularLinkedList.hpp"

template <typename T>
CircularLinkedList<T>::Node::Node(const T& val) : data(val), prev(nullptr), next(nullptr) {}

template <typename T>
CircularLinkedList<T>::Node::Node(T&& val) : data(std::move(val)), prev(nullptr), next(nullptr) {}

template <typename T>
CircularLinkedList<T>::CircularLinkedList() : head(nullptr), sz(0) {}

template <typename T>
CircularLinkedList<T>::CircularLinkedList(const CircularLinkedList& other) : head(nullptr), sz(0) {
    copy_from(other);
}

template <typename T>
CircularLinkedList<T>::CircularLinkedList(CircularLinkedList&& other) noexcept
    : head(other.head), sz(other.sz) {
    other.head = nullptr;
    other.sz = 0;
}

template <typename T>
CircularLinkedList<T>::~CircularLinkedList() {
    clear();
}

template <typename T>
void CircularLinkedList<T>::copy_from(const CircularLinkedList& other) {
    clear();
    if (other.empty()) return;
    Node* current = other.head;
    do {
        push_back(current->data);
        current = current->next;
    } while (current != other.head);
}

template <typename T>
CircularLinkedList<T>& CircularLinkedList<T>::operator=(const CircularLinkedList& other) {
    if (this != &other) {
        copy_from(other);
    }
    
    return *this;
}

template <typename T>
CircularLinkedList<T>& CircularLinkedList<T>::operator=(CircularLinkedList&& other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        sz = other.sz;
        other.head = nullptr;
        other.sz = 0;
    }
    
    return *this;
}

template <typename T>
void CircularLinkedList<T>::push_back(const T& value) {
    Node* new_node = new Node(value);
    if (empty()) {
        head = new_node;
        head->next = head;
        head->prev = head;
    } 
    else {
        Node* tail = head->prev;
        tail->next = new_node;
        new_node->prev = tail;
        new_node->next = head;
        head->prev = new_node;
    }
 
    ++sz;
}

template <typename T>
void CircularLinkedList<T>::push_front(const T& value) {
    Node* new_node = new Node(value);
    if (empty()) {
        head = new_node;
        head->next = head;
        head->prev = head;
    } 
    else {
        Node* tail = head->prev;
        new_node->next = head;
        new_node->prev = tail;
        tail->next = new_node;
        head->prev = new_node;
        head = new_node;
    }
    
    ++sz;
}

template <typename T>
void CircularLinkedList<T>::pop_back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    if (sz == 1) {
        delete head;
        head = nullptr;
    } 
    else {
        Node* tail = head->prev;
        Node* prev = tail->prev;
        prev->next = head;
        head->prev = prev;
        delete tail;
    }
    
    --sz;
}

template <typename T>
void CircularLinkedList<T>::pop_front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    if (sz == 1) {
        delete head;
        head = nullptr;
    } 
    else {
        Node* to_delete = head;
        Node* new_head = head->next;
        Node* tail = head->prev;
        new_head->prev = tail;
        tail->next = new_head;
        head = new_head;
        delete to_delete;
    }
    
    --sz;
}

template <typename T>
T& CircularLinkedList<T>::front() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return head->data;
}

template <typename T>
const T& CircularLinkedList<T>::front() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return head->data;
}

template <typename T>
T& CircularLinkedList<T>::back() {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return head->prev->data;
}

template <typename T>
const T& CircularLinkedList<T>::back() const {
    if (empty()) {
        throw std::out_of_range("List is empty");
    }
    
    return head->prev->data;
}

template <typename T>
size_t CircularLinkedList<T>::size() const {
    return sz;
}

template <typename T>
bool CircularLinkedList<T>::empty() const {
    return sz == 0;
}

template <typename T>
void CircularLinkedList<T>::clear() {
    if (empty()) return;
    Node* current = head->next;
    while (current != head) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    
    delete head;
    head = nullptr;
    sz = 0;
}

#endif