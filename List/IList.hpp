#ifndef ILIST_HPP
#define ILIST_HPP

#include <cstddef>

template <typename T>
class IList {
public:
    virtual ~IList() = default;

    virtual void push_back(const T& value) = 0;
    virtual void push_front(const T& value) = 0;
    virtual void pop_back() = 0;
    virtual void pop_front() = 0;
    virtual T& front() = 0;
    virtual const T& front() const = 0;
    virtual T& back() = 0;
    virtual const T& back() const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual void clear() = 0;
};

#endif