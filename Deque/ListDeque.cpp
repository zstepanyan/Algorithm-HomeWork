#include "ListDeque.h"

template <typename T>
LinkedListDeque<T>::LinkedListDeque() : head(nullptr), tail(nullptr), count(0) {}

template <typename T>
LinkedListDeque<T>::~LinkedListDeque() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
LinkedListDeque<T>::LinkedListDeque(const LinkedListDeque& other) : head(nullptr), tail(nullptr), count(0) {
    Node* current = other.head;
    while (current) {
        push_back(current->data);
        current = current->next;
    }
}

template <typename T>
LinkedListDeque<T>& LinkedListDeque<T>::operator=(const LinkedListDeque& other) {
    if (this != &other) {
        LinkedListDeque temp(other); 
        std::swap(head, temp.head);
        std::swap(tail, temp.tail);
        std::swap(count, temp.count);
    }

    return *this;
}

template <typename T>
LinkedListDeque<T>::LinkedListDeque(LinkedListDeque&& other) noexcept
    : head(other.head), tail(other.tail), count(other.count) {
    other.head = nullptr;
    other.tail = nullptr;
    other.count = 0;
}

template <typename T>
LinkedListDeque<T>& LinkedListDeque<T>::operator=(LinkedListDeque&& other) noexcept {
    if (this != &other) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        head = other.head;
        tail = other.tail;
        count = other.count;
        other.head = nullptr;
        other.tail = nullptr;
        other.count = 0;
    }

    return *this;
}

template <typename T>
void LinkedListDeque<T>::push_front(const T& value) {
    Node* new_node = new Node(value);
    new_node->next = head;
    head = new_node;
    if (!tail) 
        tail = head;

    count++;
}

template <typename T>
void LinkedListDeque<T>::push_back(const T& value) {
    Node* new_node = new Node(value);
    if (tail) {
        tail->next = new_node;
        tail = new_node;
    } 
    else {
        head = tail = new_node;
    }
    
    count++;
}

template <typename T>
void LinkedListDeque<T>::pop_front() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    Node* temp = head;
    head = head->next;
    if (!head) 
        tail = nullptr;

    delete temp;
    count--;
}

template <typename T>
void LinkedListDeque<T>::pop_back() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");
    if (head == tail) {
        delete head;
        head = tail = nullptr;
    } 
    else {
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
    
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
    
    count--;
}

template <typename T>
T& LinkedListDeque<T>::front() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    return head->data;
}

template <typename T>
T& LinkedListDeque<T>::back() {
    if (empty()) 
        throw std::out_of_range("Deque is empty");

    return tail->data;
}

template <typename T>
bool LinkedListDeque<T>::empty() const {
    return count == 0;
}

template <typename T>
std::size_t LinkedListDeque<T>::size() const {
    return count;
}
