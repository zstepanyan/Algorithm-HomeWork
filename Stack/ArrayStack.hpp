#ifndef STACKARRAY_HPP
#define STACKARRAY_HPP

#include "IStack.hpp"
#include <vector>
#include <stdexcept>

template <typename T>
class ArrayStack : public IStack<T> {
public:
    using value_type = typename IStack<T>::value_type;
    using reference = typename IStack<T>::reference;
    using const_reference = typename IStack<T>::const_reference;
    using size_type = typename IStack<T>::size_type;

    ArrayStack();
    ~ArrayStack() override;
    ArrayStack(const ArrayStack& other);
    ArrayStack& operator=(const ArrayStack& other);
    ArrayStack(ArrayStack&& other) noexcept;
    ArrayStack& operator=(ArrayStack&& other) noexcept;

    void push(const_reference element) override;
    void emplace(value_type&& element) override;
    const_reference top() const override;
    reference top() override;
    bool empty() const override;
    value_type peek() const override;
    void pop() override;
    size_type size() const override;

private:
    std::vector<value_type> elements;
};

#include "ArrayStack.tpp"

#endif // STACKARRAY_HPP