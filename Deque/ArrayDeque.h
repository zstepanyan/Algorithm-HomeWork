#ifndef ARRAY_DEQUE_H
#define ARRAY_DEQUE_H

#include "IDeque.h"
#include <vector>

template <typename T>
class ArrayDeque : public IDeque<T> {
public:
    ArrayDeque(size_t initial_capacity = 8);
    ~ArrayDeque() override = default; // vector handles cleanup
    ArrayDeque(const ArrayDeque& other) = default; 
    ArrayDeque& operator=(const ArrayDeque& other) = default; 
    ArrayDeque(ArrayDeque&& other) noexcept = default; 
    ArrayDeque& operator=(ArrayDeque&& other) noexcept = default; 

    void push_front(const T& value) override;
    void push_back(const T& value) override;
    void pop_front() override;
    void pop_back() override;
    T& front() override;
    T& back() override;
    bool empty() const override;
    size_t size() const override;

private:
    void reserve_if_needed();

    std::vector<T> array;
    size_t count;
    size_t front_idx;
    size_t back_idx;
};

#endif // ARRAY_DEQUE_H