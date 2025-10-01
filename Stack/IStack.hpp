#ifndef ISTACK_HPP
#define ISTACK_HPP

template <typename T>
class IStack {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    virtual ~IStack() = default;
    virtual void push(const_reference element) = 0;
    virtual void emplace(value_type&& element) = 0;
    virtual const_reference top() const = 0;
    virtual reference top() = 0;
    virtual bool empty() const = 0;
    virtual value_type peek() const = 0;
    virtual void pop() = 0;
    virtual size_type size() const = 0;
};

#endif // ISTACK_HPP