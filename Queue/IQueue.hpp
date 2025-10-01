#ifndef IQUEUE_HPP
#define IQUEUE_HPP

template <typename T>
class IQueue {
public:
    virtual ~IQueue()=default;
    virtual void enqueue(const T&) = 0;
    virtual void dequeue() = 0;
    virtual std::size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual T& front() = 0;
    virtual const T& front() const = 0;
    virtual T& back() = 0;
    virtual const T& back() const = 0;
    virtual void emplace(const T&) = 0;
};

#endif // IQUEUE_HPP