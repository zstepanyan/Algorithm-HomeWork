#ifndef AVL_H
#define AVL_H

#include <utility>
#include <functional>
#include <initializer_list>
#include <cstdint>

namespace {
    template <typename T>
    struct node_t {
        node_t* parent;
        node_t* left;
        node_t* right;
        T val;
        uint16_t height;
        node_t(const T& value) : parent(nullptr), left(nullptr), right(nullptr), val(value), height(1) {}
        node_t(T&& value) : parent(nullptr), left(nullptr), right(nullptr), val(value), height(1) {}
    };
}

template <typename T, typename Comp = std::less<T>>
class AVL {
public:
    using node = node_t<T>;
    using value_type = T;

    AVL();
    AVL(const AVL& rhs);
    AVL(AVL&& rhs) noexcept;
    AVL(std::initializer_list<T> init);
    ~AVL();

    AVL& operator=(const AVL& rhs);
    AVL& operator=(AVL&& rhs) noexcept;

    void left_rotate(node* x);
    void right_rotate(node* x);
    void left_right_rotate(node* x);
    void right_left_rotate(node* x);

    void insert(const T& key);
    void insert(T&& val);
    template <typename... Args>
    void emplace(Args&&... args);

    void delete_node(const T& val);
    void delete_node(node* nd);

    node* find(const T& val) const;
    node* find(T&& val) const;

    bool contains(const T& val) const;
    bool contains(T&& val) const;

    void pre_order() const;
    void in_order() const;
    void post_order() const;

    bool empty() const;
    void clear();

private:
    node* root_;

    uint16_t height(node* nd) const;
    void update_height(node* nd);
    int balance_factor(node* nd) const;
    node* copy_tree(node* nd, node* parent);
    void clear_tree(node* nd);
    void insert_node(node* new_node);
    void insert_fixup(node* nd);
    node* min_node(node* nd) const;
    void pre_order(node* nd) const;
    void in_order(node* nd) const;
    void post_order(node* nd) const;
};


#endif // AVL_H