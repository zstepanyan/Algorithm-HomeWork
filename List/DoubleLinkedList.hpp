#ifndef DOUBLE_LINKED_LIST_HPP
#define DOUBLE_LINKED_LIST_HPP

#include "IList.hpp"
#include <stdexcept>
#include <utility>

template <typename T>
class DoubleLinkedList : public IList<T> {
public:
    DoubleLinkedList();
    DoubleLinkedList(const DoubleLinkedList& other);
    DoubleLinkedList(DoubleLinkedList&& other) noexcept;
    ~DoubleLinkedList();

    DoubleLinkedList& operator=(const DoubleLinkedList& other);
    DoubleLinkedList& operator=(DoubleLinkedList&& other) noexcept;

    void push_back(const T& value) override;
    void push_front(const T& value) override;
    void pop_back() override;
    void pop_front() override;

    T& front() override;
    const T& front() const override;
    T& back() override;
    const T& back() const override;
    
    size_t size() const override;
    bool empty() const override;
    void clear() override;

private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& val);
        Node(T&& val);
    };

    Node* head;
    Node* tail;
    size_t sz;

    void copy_from(const DoubleLinkedList& other);
};

#include "DoubleLinkedList.tpp" 

#endif