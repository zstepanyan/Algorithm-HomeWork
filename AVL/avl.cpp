#include "avl.h"
#include <algorithm>
#include <iostream>

template <typename T, typename Comp>
AVL<T, Comp>::AVL() : root_(nullptr) {}

template <typename T, typename Comp>
AVL<T, Comp>::AVL(const AVL<T, Comp>& rhs) : root_(nullptr) {
    *this = rhs;
}

template <typename T, typename Comp>
AVL<T, Comp>::AVL(AVL&& rhs) noexcept : root_(rhs.root_) {
    rhs.root_ = nullptr;
}

template <typename T, typename Comp>
AVL<T, Comp>::AVL(std::initializer_list<T> init) : root_(nullptr) {
    for (const auto& val : init) {
        insert(val);
    }
}

template <typename T, typename Comp>
AVL<T, Comp>::~AVL() {
    clear();
}

template <typename T, typename Comp>
AVL<T, Comp>& AVL<T, Comp>::operator=(const AVL& rhs) {
    if (this != &rhs) {
        clear();
        if (rhs.root_) {
            root_ = copy_tree(rhs.root_, nullptr);
        }
    }
    
    return *this;
}

template <typename T, typename Comp>
AVL<T, Comp>& AVL<T, Comp>::operator=(AVL&& rhs) noexcept {
    if (this != &rhs) {
        clear();
        root_ = rhs.root_;
        rhs.root_ = nullptr;
    }
    
    return *this;
}

template <typename T, typename Comp>
void AVL<T, Comp>::left_rotate(node* x) {
    if (!x || !x->right) {
        return;
    }

    node* y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (!x->parent) {
        root_ = y;
    } 
    else if (x == x->parent->left) {
        x->parent->left = y;
    } 
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
    update_height(x);
    update_height(y);
}

template <typename T, typename Comp>
void AVL<T, Comp>::right_rotate(node* x) {
    if (!x || !x->left) {
        return;
    }

    node* y = x->left;
    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (!x->parent) {
        root_ = y;
    } 
    else if (x == x->parent->right) {
        x->parent->right = y;
    } 
    else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
    update_height(x);
    update_height(y);
}

template <typename T, typename Comp>
void AVL<T, Comp>::left_right_rotate(node* x) {
    if (!x || !x->left) {
        return;
    }
    
    left_rotate(x->left);
    right_rotate(x);
}

template <typename T, typename Comp>
void AVL<T, Comp>::right_left_rotate(node* x) {
    if (!x || !x->right) {
        return;
    }
    
    right_rotate(x->right);
    left_rotate(x);
}

template <typename T, typename Comp>
void AVL<T, Comp>::insert(const T& key) {
    node* new_node = new node_t<T>(key);
    insert_node(new_node);
}

template <typename T, typename Comp>
void AVL<T, Comp>::insert(T&& val) {
    node* new_node = new node_t<T>(std::move(val));
    insert_node(new_node);
}

template <typename T, typename Comp>
template <typename... Args>
void AVL<T, Comp>::emplace(Args&&... args) {
    node* new_node = new node_t<T>(T(std::forward<Args>(args)...));
    insert_node(new_node);
}

template <typename T, typename Comp>
void AVL<T, Comp>::delete_node(const T& val) {
    node* nd = find(val);
    if (nd) {
        delete_node(nd);
    }
}

template <typename T, typename Comp>
void AVL<T, Comp>::delete_node(node* nd) {
    if (!nd) {
        return;
    }

    node* parent = nd->parent;
    if (!nd->left && !nd->right) { // Leaf 
        if (nd == root_) {
            root_ = nullptr;
        } 
        else if (nd == parent->left) {
            parent->left = nullptr;
        } 
        else {
            parent->right = nullptr;
        }
        
        delete nd;
    } 
    else if (!nd->left || !nd->right) { // One child
        node* child = nd->left ? nd->left : nd->right;
        if (nd == root_) {
            root_ = child;
        } 
        else if (nd == parent->left) {
            parent->left = child;
        } 
        else {
            parent->right = child;
        }
        
        child->parent = parent;
        delete nd;
    } 
    else { // Two children
        node* successor = min_node(nd->right);
        nd->val = std::move(successor->val);
        delete_node(successor);
        return;
    }
    
    if (parent) {
        insert_fixup(parent);
    }
}

template <typename T, typename Comp>
typename AVL<T, Comp>::node* AVL<T, Comp>::find(const T& val) const {
    node* current = root_;
    Comp comp;
    while (current) {
        if (comp(val, current->val)) {
            current = current->left;
        } 
        else if (comp(current->val, val)) {
            current = current->right;
        } 
        else {
            return current;
        }
    }
    
    return nullptr;
}

template <typename T, typename Comp>
typename AVL<T, Comp>::node* AVL<T, Comp>::find(T&& val) const {
    return find(val);
}

template <typename T, typename Comp>
bool AVL<T, Comp>::contains(const T& val) const {
    return find(val) != nullptr;
}

template <typename T, typename Comp>
bool AVL<T, Comp>::contains(T&& val) const {
    return find(val) != nullptr;
}

template <typename T, typename Comp>
void AVL<T, Comp>::pre_order() const {
    pre_order(root_);
}

template <typename T, typename Comp>
void AVL<T, Comp>::in_order() const {
    in_order(root_);
}

template <typename T, typename Comp>
void AVL<T, Comp>::post_order() const {
    post_order(root_);
}

template <typename T, typename Comp>
bool AVL<T, Comp>::empty() const {
    return root_ == nullptr;
}

template <typename T, typename Comp>
void AVL<T, Comp>::clear() {
    clear_tree(root_);
    root_ = nullptr;
}

template <typename T, typename Comp>
uint16_t AVL<T, Comp>::height(node* nd) const {
    return nd == nullptr ? nd->height : 0;
}

template <typename T, typename Comp>
void AVL<T, Comp>::update_height(node* nd) {
    if (nd != nullptr) {
        nd->height = 1 + std::max(height(nd->left), height(nd->right));
    }
}

template <typename T, typename Comp>
int AVL<T, Comp>::balance_factor(node* nd) const {
    return nd == nullptr ? height(nd->left) - height(nd->right) : 0;
}

template <typename T, typename Comp>
typename AVL<T, Comp>::node* AVL<T, Comp>::copy_tree(node* nd, node* parent) {
    if (!nd) {
        return nullptr;
    }
    
    node* new_node = new node_t<T>(nd->val);
    new_node->parent = parent;
    new_node->height = nd->height;
    new_node->left = copy_tree(nd->left, new_node);
    new_node->right = copy_tree(nd->right, new_node);
    return new_node;
}

template <typename T, typename Comp>
void AVL<T, Comp>::clear_tree(node* nd) {
    if (!nd) {
        return;
    }
    
    clear_tree(nd->left);
    clear_tree(nd->right);
    delete nd;
}

template <typename T, typename Comp>
void AVL<T, Comp>::insert_node(node* new_node) {
    if (!root_) {
        root_ = new_node;
        return;
    }

    node* current = root_;
    node* parent = nullptr;
    Comp comp;
    while (current != nullptr) {
        parent = current;
        if (comp(new_node->val, current->val)) {
            current = current->left;
        } 
        else if (comp(current->val, new_node->val)) {
            current = current->right;
        } 
        else {
            delete new_node; // duplicate value, do not insert
            return;
        }
    }

    new_node->parent = parent;
    if (comp(new_node->val, parent->val)) {
        parent->left = new_node;
    } 
    else {
        parent->right = new_node;
    }
    
    insert_fixup(new_node);
}

template <typename T, typename Comp>
void AVL<T, Comp>::insert_fixup(node* nd) {
    while (nd != nullptr) {
        update_height(nd);
        int bf = balance_factor(nd);
        if (bf > 1) {
            if (balance_factor(nd->left) < 0) {
                left_right_rotate(nd);
            } 
            else {
                right_rotate(nd);
            }
        } 
        else if (bf < -1) {
            if (balance_factor(nd->right) > 0) {
                right_left_rotate(nd);
            } 
            else {
                left_rotate(nd);
            }
        }

        nd = nd->parent;
    }
}

template <typename T, typename Comp>
typename AVL<T, Comp>::node* AVL<T, Comp>::min_node(node* nd) const {
    while (nd && nd->left) {
        nd = nd->left;
    }

    return nd;
}

template <typename T, typename Comp>
void AVL<T, Comp>::pre_order(node* nd) const {
    if (nd) {
        std::cout << nd->val << " ";
        pre_order(nd->left);
        pre_order(nd->right);
    }
}

template <typename T, typename Comp>
void AVL<T, Comp>::in_order(node* nd) const {
    if (nd) {
        in_order(nd->left);
        std::cout << nd->val << " ";
        in_order(nd->right);
    }
}

template <typename T, typename Comp>
void AVL<T, Comp>::post_order(node* nd) const {
    if (nd) {
        post_order(nd->left);
        post_order(nd->right);
        std::cout << nd->val << " ";
    }
}

template class AVL<int>;
template class AVL<double>;
template class AVL<std::string>;