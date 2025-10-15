#ifndef IDEQUE_H
#define IDEQUE_H

#include <cstddef>

template <typename T>
class IDeque {
public:
    virtual ~IDeque() = default;
    virtual void push_front(const T& value) = 0;
    virtual void push_back(const T& value) = 0;
    virtual void pop_front() = 0;
    virtual void pop_back() = 0;
    virtual T& front() = 0;
    virtual T& back() = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
};

#endif // IDEQUE_H