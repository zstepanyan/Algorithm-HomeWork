#ifndef CIRCULAR_LINKED_LIST_HPP
#define CIRCULAR_LINKED_LIST_HPP

#include "IList.hpp"
#include <stdexcept>
#include <utility>

template <typename T>
class CircularLinkedList : public IList<T> {
public:
    CircularLinkedList();
    CircularLinkedList(const CircularLinkedList& other);
    CircularLinkedList(CircularLinkedList&& other) noexcept;
    ~CircularLinkedList();

    CircularLinkedList& operator=(const CircularLinkedList& other);
    CircularLinkedList& operator=(CircularLinkedList&& other) noexcept;

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
    size_t sz;

    void copy_from(const CircularLinkedList& other);
};

#include "CircularLinkedList.tpp" 

#endif