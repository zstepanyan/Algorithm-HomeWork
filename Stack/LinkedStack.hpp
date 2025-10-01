#ifndef STACKLIST_HPP
#define STACKLIST_HPP

#include "IStack.hpp"
#include <list>
#include <stdexcept>

template <typename T>
class LinkedStack : public IStack<T> {
public:
    using value_type = typename IStack<T>::value_type;
    using reference = typename IStack<T>::reference;
    using const_reference = typename IStack<T>::const_reference;
    using size_type = typename IStack<T>::size_type;

    LinkedStack();
    ~LinkedStack() override;
    LinkedStack(const LinkedStack& other);
    LinkedStack& operator=(const LinkedStack& other);
    LinkedStack(LinkedStack&& other) noexcept;
    LinkedStack& operator=(LinkedStack&& other) noexcept;

    void push(const_reference element) override;
    void emplace(value_type&& element) override;
    const_reference top() const override;
    reference top() override;
    bool empty() const override;
    value_type peek() const override;
    void pop() override;
    size_type size() const override;

private:
    std::list<value_type> elements_;
};

#include "LinkedStack.tpp"

#endif // STACK_LIST_HPP