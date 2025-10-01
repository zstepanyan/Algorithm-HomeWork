#ifndef LIST_QUEUE_HPP
#define LIST_QUEUE_HPP

#include "IQueue.hpp"
#include <memory>
#include <cstddef>
#include <stdexcept>

template <typename T>
class ListQueue : public IQueue<T> {
public:
    ListQueue();
    ListQueue(const ListQueue& rhs);
    ListQueue& operator=(const ListQueue& rhs);
    ListQueue(ListQueue&& rhs) noexcept;
    ListQueue& operator=(ListQueue&& rhs) noexcept;
    ~ListQueue() override;

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
    struct Node {
        T val;
        std::unique_ptr<Node> next;
        Node(const T& val) : val(val) {}
    };
    
    std::unique_ptr<Node> front_;
    std::unique_ptr<Node> back_;
    std::size_t size_;
};

#include "ListQueue.tpp"

#endif // LIST_QUEUE_HPP