#include "ArrayQueue.hpp"

template <typename T>
ArrayQueue<T>::ArrayQueue() : data_(), front_(0), rear_(0) {}

template <typename T>
ArrayQueue<T>::~ArrayQueue() {}

template <typename T>
ArrayQueue<T>::ArrayQueue(const ArrayQueue& rhs) : front_(0), rear_(rhs.size()) {
    if (rhs.empty()) 
        return;

    data_.resize(rhs.size() * 2);
    std::size_t j = rhs.front_;
    for (std::size_t i = 0; i < rhs.size(); ++i) {
        data_[i] = rhs.data_[j];
        j = (j + 1) % rhs.data_.size();
    }
}

template <typename T>
ArrayQueue<T>& ArrayQueue<T>::operator=(const ArrayQueue& rhs) {
    if (this == &rhs) 
        return *this;

    data_.clear();
    front_ = 0;
    rear_ = 0;
    if (rhs.empty()) 
        return *this;
        
    data_.resize(rhs.size() * 2);
    std::size_t j = rhs.front_;
    for (std::size_t i = 0; i < rhs.size(); ++i) {
        data_[i] = rhs.data_[j];
        j = (j + 1) % rhs.data_.size();
    }

    rear_ = rhs.size();
    return *this;
}

template <typename T>
ArrayQueue<T>::ArrayQueue(ArrayQueue&& rhs) noexcept : data_(std::move(rhs.data_)), front_(rhs.front_), rear_(rhs.rear_) {
    rhs.front_ = 0;
    rhs.rear_ = 0;
}

template <typename T>
ArrayQueue<T>& ArrayQueue<T>::operator=(ArrayQueue&& rhs) noexcept {
    if (this == &rhs) 
        return *this;
    
    data_ = std::move(rhs.data_);
    front_ = rhs.front_;
    rear_ = rhs.rear_;
    rhs.front_ = 0;
    rhs.rear_ = 0;
    
    return *this;
}

template <typename T>
void ArrayQueue<T>::enqueue(const T& val) {
    if ((rear_ + 1) % (data_.size() == 0 ? 1 : data_.size()) == front_) {
        std::vector<T> new_data(data_.empty() ? 4 : data_.size() * 2);
        std::size_t new_rear = 0;
        std::size_t i = front_;
        while (i != rear_) {
            new_data[new_rear++] = std::move(data_[i]);
            i = (i + 1) % data_.size();
        }
    
        data_ = std::move(new_data);
        front_ = 0;
        rear_ = new_rear;
    }
    
    if (rear_ < data_.size()) {
        data_[rear_] = val;
    } 
    else {
        data_.push_back(val);
    }
    
    rear_ = (rear_ + 1) % (data_.size() == 0 ? 1 : data_.size());
}

template <typename T>
void ArrayQueue<T>::dequeue() {
    if (empty()) {
        throw std::out_of_range("Dequeue from empty queue");
    }
    
    front_ = (front_ + 1) % (data_.size() == 0 ? 1 : data_.size());
    if (front_ == rear_) {
        front_ = 0;
        rear_ = 0;
        data_.clear();
    }
}

template <typename T>
std::size_t ArrayQueue<T>::size() const {
    return (rear_ + data_.size() - front_) % (data_.size() == 0 ? 1 : data_.size());
}

template <typename T>
bool ArrayQueue<T>::empty() const {
    return front_ == rear_;
}

template <typename T>
T& ArrayQueue<T>::front() {
    if (empty()) {
        throw std::out_of_range("Front of empty queue");
    }
    
    return data_[front_];
}

template <typename T>
const T& ArrayQueue<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Front of empty queue");
    }
    
    return data_[front_];
}

template <typename T>
T& ArrayQueue<T>::back() {
    if (empty()) {
        throw std::out_of_range("Back of empty queue");
    }
    
    std::size_t idx = (rear_ - 1 + data_.size()) % (data_.size() == 0 ? 1 : data_.size());
    return data_[idx];
}

template <typename T>
const T& ArrayQueue<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Back of empty queue");
    }
    
    std::size_t idx = (rear_ - 1 + data_.size()) % (data_.size() == 0 ? 1 : data_.size());
    return data_[idx];
}

template <typename T>
void ArrayQueue<T>::emplace(const T& val) {
    enqueue(val);
}