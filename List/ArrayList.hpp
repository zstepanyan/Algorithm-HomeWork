#ifndef ARRAY_LIST_HPP
#define ARRAY_LIST_HPP

#include "IList.hpp"
#include <stdexcept>
#include <utility>

template <typename T>
class ArrayList : public IList<T> {
public:
    ArrayList();
    ArrayList(const ArrayList& other);
    ArrayList(ArrayList&& other) noexcept;
    ~ArrayList();

    ArrayList& operator=(const ArrayList& other);
    ArrayList& operator=(ArrayList&& other) noexcept;

    void push_back(const T& value) override;
    void push_front(const T& value) override;
    void pop_back() override;
    void pop_front() override;
    
    T& front() override;
    const T& front() const override;
    T& back() override;
    const T& back() const override;

    size_t size() const override;
    bool empty() const override;
    void clear() override;

private:
    T* arr;
    size_t capacity;
    size_t sz;

    void resize(size_t new_capacity);
};

#include "ArrayList.tpp" 

#endif