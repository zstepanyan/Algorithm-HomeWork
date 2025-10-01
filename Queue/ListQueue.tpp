#include "ListQueue.hpp"

template <typename T>
ListQueue<T>::ListQueue() : front_(nullptr), back_(nullptr), size_(0) {}

template <typename T>
ListQueue<T>::ListQueue(const ListQueue& rhs) : size_(rhs.size_) {
    if (rhs.empty()) {
        front_ = nullptr;
        back_ = nullptr;
        return;
    }

    front_ = std::make_unique<Node>(rhs.front_->val);
    auto* current = front_.get();
    auto* rhs_current = rhs.front_->next.get();
    while (rhs_current) {
        current->next = std::make_unique<Node>(rhs_current->val);
        current = current->next.get();
        rhs_current = rhs_current->next.get();
    }
    
    if (current == front_.get()) {
        back_ = std::move(front_);
    } 
    else {
        back_ = std::make_unique<Node>(current->val);
    }
}

template <typename T>
ListQueue<T>& ListQueue<T>::operator=(const ListQueue& rhs) {
    if (this == &rhs) 
        return *this;
    
    front_ = nullptr;
    back_ = nullptr;
    size_ = 0;
    if (rhs.empty()) 
        return *this;

    front_ = std::make_unique<Node>(rhs.front_->val);
    auto* current = front_.get();
    auto* rhs_current = rhs.front_->next.get();
    while (rhs_current) {
        current->next = std::make_unique<Node>(rhs_current->val);
        current = current->next.get();
        rhs_current = rhs_current->next.get();
    }

    if (current == front_.get()) {
        back_ = std::move(front_);
    } 
    else {
        back_ = std::make_unique<Node>(current->val);
    }
    
    size_ = rhs.size_;
    return *this;
}

template <typename T>
ListQueue<T>::ListQueue(ListQueue&& rhs) noexcept : front_(std::move(rhs.front_)), back_(std::move(rhs.back_)), size_(rhs.size_) {
    rhs.size_ = 0;
}

template <typename T>
ListQueue<T>& ListQueue<T>::operator=(ListQueue&& rhs) noexcept {
    if (this == &rhs) 
        return *this;

    front_ = std::move(rhs.front_);
    back_ = std::move(rhs.back_);
    size_ = rhs.size_;
    rhs.size_ = 0;

    return *this;
}

template <typename T>
ListQueue<T>::~ListQueue() {}

template <typename T>
void ListQueue<T>::enqueue(const T& val) {
    auto new_node = std::make_unique<Node>(val);
    if (empty()) {
        front_ = std::move(new_node);
        back_ = std::unique_ptr<Node>(front_.get());
    } 
    else {
        back_->next = std::move(new_node);
        back_ = std::unique_ptr<Node>(back_->next.get());
    }
    
    ++size_;
}

template <typename T>
void ListQueue<T>::dequeue() {
    if (empty()) {
        throw std::out_of_range("Dequeue from empty queue");
    }
    
    front_ = std::move(front_->next);
    if (!front_) {
        back_ = nullptr;
    }
    
    --size_;
}

template <typename T>
std::size_t ListQueue<T>::size() const {
    return size_;
}

template <typename T>
bool ListQueue<T>::empty() const {
    return size_ == 0;
}

template <typename T>
T& ListQueue<T>::front() {
    if (empty()) {
        throw std::out_of_range("Front of empty queue");
    }
    
    return front_->val;
}

template <typename T>
const T& ListQueue<T>::front() const {
    if (empty()) {
        throw std::out_of_range("Front of empty queue");
    }
    
    return front_->val;
}

template <typename T>
T& ListQueue<T>::back() {
    if (empty()) {
        throw std::out_of_range("Back of empty queue");
    }
    
    return back_->val;
}

template <typename T>
const T& ListQueue<T>::back() const {
    if (empty()) {
        throw std::out_of_range("Back of empty queue");
    }
    
    return back_->val;
}

template <typename T>
void ListQueue<T>::emplace(const T& val) {
    auto new_node = std::make_unique<Node>(val);
    if (empty()) {
        front_ = std::move(new_node);
        back_ = std::unique_ptr<Node>(front_.get());
    } 
    else {
        back_->next = std::move(new_node);
        back_ = std::unique_ptr<Node>(back_->next.get());
    }
 
    ++size_;
}