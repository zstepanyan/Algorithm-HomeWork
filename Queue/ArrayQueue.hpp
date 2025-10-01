#ifndef ARRAY_QUEUE_HPP
#define ARRAY_QUEUE_HPP

#include "IQueue.hpp"
#include <vector>
#include <cstddef>
#include <stdexcept>

template <typename T>
class ArrayQueue : public IQueue<T> {
public:
    ArrayQueue();
    ArrayQueue(const ArrayQueue& rhs);
    ArrayQueue& operator=(const ArrayQueue& rhs);
    ArrayQueue(ArrayQueue&& rhs) noexcept;
    ArrayQueue& operator=(ArrayQueue&& rhs) noexcept;
    ~ArrayQueue() override;

    void enqueue(const T& val) override;
    void dequeue() override;
    std::size_t size() const override;
    bool empty() const override;
    T& front() override;
    const T& front() const override;
    T& back() override;
    const T& back() const override;
    void emplace(const T&);
     
private:
    std::vector<T> data_;
    std::size_t front_;
    std::size_t rear_;
};

#include "ArrayQueue.tpp"

#endif // ARRAY_QUEUE_HPP