#pragma once

#include "DynamicArray.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class AVLTree
{
private:
    struct Node
    {
        T value;
        Node *left;
        Node *right;
        int height;

        Node(const T &value)
            : value{value}, left{nullptr}, right{nullptr}, height{1}
        {
        }
    };

    Node *root_;
    std::size_t size_;

    int height(const Node *node) const noexcept
    {
        if (node == nullptr)
        {
            return 0;
        }

        return node->height;
    }

    void update_height(Node *node) noexcept
    {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    int balance_factor(const Node *node) const noexcept
    {
        if (node == nullptr)
        {
            return 0;
        }

        return height(node->left) - height(node->right);
    }

    Node *rotate_right(Node *y) noexcept
    {
        Node *x{y->left};
        Node *sub_B{x->right};

        x->right = y;
        y->left = sub_B;

        update_height(y);
        update_height(x);

        return x;
    }

    Node *rotate_left(Node *y) noexcept
    {
        Node *x{y->right};
        Node *sub_B{x->left};

        x->left = y;
        y->right = sub_B;

        update_height(y);
        update_height(x);

        return x;
    }

    Node *insert(Node *node, const T &value)
    {
        if (node == nullptr)
        {
            Node *new_node = new Node{value};
            ++size_;

            return new_node;
        }

        if (value < node->value)
        {
            node->left = insert(node->left, value);
        }
        else if (value > node->value)
        {
            node->right = insert(node->right, value);
        }
        else
        {
            return node;
        }

        update_height(node);
        int balance{balance_factor(node)};

        if (balance > 1 && value < node->left->value)
        {
            return rotate_right(node);
        }
        else if (balance > 1 && value > node->left->value)
        {
            node->left = rotate_left(node->left);
            return rotate_right(node);
        }
        else if (balance < -1 && value > node->right->value)
        {
            return rotate_left(node);
        }
        else if (balance < -1 && value < node->right->value)
        {
            node->right = rotate_right(node->right);
            return rotate_left(node);
        }

        return node;
    }

    void preorder(const Node *node, DynamicArray<T> &result) const
    {
        if (node == nullptr)
        {
            return;
        }

        result.push_back(node->value);

        preorder(node->left, result);

        preorder(node->right, result);
    }

    void inorder(const Node *node, DynamicArray<T> &result) const
    {
        if (node == nullptr)
        {
            return;
        }

        inorder(node->left, result);

        result.push_back(node->value);

        inorder(node->right, result);
    }

    void postorder(const Node *node, DynamicArray<T> &result) const
    {
        if (node == nullptr)
        {
            return;
        }

        postorder(node->left, result);
        postorder(node->right, result);
        result.push_back(node->value);
    }

    void clear(Node *node) noexcept
    {
        if (node == nullptr)
        {
            return;
        }

        clear(node->left);
        clear(node->right);

        delete node;
    }

    Node *clone(const Node *node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        Node *new_node = new Node{node->value};

        try
        {
            new_node->left = clone(node->left);
            new_node->right = clone(node->right);

            update_height(new_node);
        }
        catch (...)
        {
            clear(new_node);
            throw;
        }

        return new_node;
    }

    void swap(AVLTree &other) noexcept
    {
        using std::swap;

        swap(root_, other.root_);
        swap(size_, other.size_);
    }

public:
    // Construction / Ownership
    AVLTree()
        : root_{nullptr}, size_{0}
    {
    }

    AVLTree(const AVLTree &other)
        : root_{clone(other.root_)}, size_{other.size_}
    {
    }

    AVLTree(AVLTree &&other) noexcept
    : root_{other.root_}, size_{other.size_}
    {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    AVLTree &operator=(const AVLTree &other)
    {
        AVLTree temp{other};

        swap(temp);

        return *this;
    }

    AVLTree &operator=(AVLTree &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        clear();

        root_ = other.root_;
        size_ = other.size_;

        other.root_ = nullptr;
        other.size_ = 0;

        return *this;
    }

    ~AVLTree()
    {
        clear();
    }

    // Capacity / State
    [[nodiscard]] bool empty() const noexcept
    {
        return size_ == 0;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return size_;
    }

    // Element Access
    [[nodiscard]] bool contains(const T &value) const
    {
        const Node *current{root_};
        while (current)
        {
            if (value < current->value)
            {
                current = current->left;
            }
            else if (value > current->value)
            {
                current = current->right;
            }
            else
            {
                return true;
            }
        }

        return false;
    }

    const T &min() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty tree.");
        }

        const Node *current{root_};

        while(current->left)
        {
            current = current->left;
        }

        return current->value;
    }

    const T &max() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty tree.");
        }

        const Node *current{root_};

        while(current->right)
        {
            current = current->right;
        }

        return current->value;
    }

    // Modifiers
    void insert(const T &value)
    {
        root_ = insert(root_, value);
    }

    void clear() noexcept
    {
        clear(root_);

        root_ = nullptr;
        size_ = 0;
    }

    // Traversal
    DynamicArray<T> preorder() const
    {
        DynamicArray<T> result{};

        preorder(root_, result);

        return result;
    }

    DynamicArray<T> inorder() const
    {
        DynamicArray<T> result{};

        inorder(root_, result);

        return result;
    }

    DynamicArray<T> postorder() const
    {
        DynamicArray<T> result{};

        postorder(root_, result);

        return result;
    }
};