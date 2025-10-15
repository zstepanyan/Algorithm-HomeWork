#ifndef LINKED_LIST_DEQUE_H
#define LINKED_LIST_DEQUE_H

#include "IDeque.h"

template <typename T>
class LinkedListDeque : public IDeque<T> {
public:
    LinkedListDeque();
    ~LinkedListDeque() override;
    LinkedListDeque(const LinkedListDeque& other); 
    LinkedListDeque& operator=(const LinkedListDeque& other); 
    LinkedListDeque(LinkedListDeque&& other) noexcept; 
    LinkedListDeque& operator=(LinkedListDeque&& other) noexcept; 

    void push_front(const T& value) override;
    void push_back(const T& value) override;
    void pop_front() override;
    void pop_back() override;
    T& front() override;
    T& back() override;
    bool empty() const override;
    size_t size() const override;

private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;
};

#endif // LINKED_LIST_DEQUE_H