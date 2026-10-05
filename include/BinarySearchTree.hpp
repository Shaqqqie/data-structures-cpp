#pragma once

#include "DynamicArray.hpp"
#include "Queue.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class BinarySearchTree
{
private:
    struct Node
    {
        T value;
        Node *left;
        Node *right;
    };

    Node *root_;
    std::size_t size_;

    void swap(BinarySearchTree &other) noexcept
    {
        using std::swap;

        swap(root_, other.root_);
        swap(size_, other.size_);
    }

    // Recursive implementation details
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

    void clear(Node *node)
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

        Node *new_node = new Node{node->value, nullptr, nullptr};

        try
        {
            new_node->left = clone(node->left);
            new_node->right = clone(node->right);
        }
        catch (...)
        {
            clear(new_node);
            throw;
        }

        return new_node;
    }

    std::size_t height(const Node *node) const
    {
        if (node == nullptr)
        {
            return 0;
        }

        std::size_t left_height{height(node->left)};
        std::size_t right_height{height(node->right)};

        return 1 + std::max(left_height, right_height);
    }

public:
    // Construction / Ownership
    BinarySearchTree()
        : root_{nullptr}, size_{0}
    {
    }

    BinarySearchTree(const BinarySearchTree &other)
        : root_{clone(other.root_)}, size_{other.size_}
    {
    }

    BinarySearchTree(BinarySearchTree &&other) noexcept
        : root_{other.root_}, size_{other.size_}
    {
        other.root_ = nullptr;
        other.size_ = 0;
    }

    BinarySearchTree &operator=(const BinarySearchTree &other)
    {
        BinarySearchTree temp{other};

        swap(temp);

        return *this;
    }

    BinarySearchTree &operator=(BinarySearchTree &&other) noexcept
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

    ~BinarySearchTree()
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

    [[nodiscard]] std::size_t height() const
    {
        return height(root_);
    }

    // Element Access
    [[nodiscard]] bool contains(const T &value) const
    {
        const Node *current{root_};

        while (current)
        {
            if (value == current->value)
            {
                return true;
            }
            else if (value < current->value)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
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

        while (current->left)
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

        while (current->right)
        {
            current = current->right;
        }

        return current->value;
    }

    // Modifiers
    void insert(const T &value)
    {
        if (empty())
        {
            Node *node = new Node{value, nullptr, nullptr};
            root_ = node;
            ++size_;

            return;
        }

        Node *current{root_};
        Node *parent{nullptr};

        while (current)
        {
            parent = current;

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
                return;
            }
        }

        Node *node = new Node{value, nullptr, nullptr};

        if (value < parent->value)
        {
            parent->left = node;
        }
        else
        {
            parent->right = node;
        }

        ++size_;
    }

    void clear()
    {
        clear(root_);

        root_ = nullptr;
        size_ = 0;
    }

    bool erase(const T &value)
    {
        if (empty())
        {
            return false;
        }

        Node *current{root_};
        Node *parent{nullptr};

        // Traversing tree
        while (current)
        {
            if (value < current->value)
            {
                parent = current;
                current = current->left;
            }
            else if (value > current->value)
            {
                parent = current;
                current = current->right;
            }
            else
            {
                break;
            }
        }

        // Value not part of tree
        if (current == nullptr)
        {
            return false;
        }

        // Leaf deletion
        if (current->left == nullptr && current->right == nullptr)
        {
            if (parent == nullptr)
            {
                delete root_;
                root_ = nullptr;
                --size_;

                return true;
            }

            if (parent->left == current)
            {
                parent->left = nullptr;
            }
            else
            {
                parent->right = nullptr;
            }

            delete current;
            --size_;

            return true;
        }

        // One child deletion
        if (current->left == nullptr || current->right == nullptr)
        {
            Node *child{current->left != nullptr ? current->left : current->right};

            if (parent == nullptr)
            {
                root_ = child;
                delete current;
                --size_;
                
                return true;
            }

            if (parent->left == current)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }

            delete current;
            --size_;
            return true;
        }

        // Two child deletion
        Node *successor_parent{current};
        Node *successor{current->right};

        while(successor->left)
        {
            successor_parent = successor;
            successor = successor->left;
        }

        current->value = successor->value;
        
        if (successor_parent == current)
        {
            successor_parent->right = successor->right;
        }
        else
        {
            successor_parent->left = successor->right;
        }

        delete successor;
        --size_;
        return true;
    }

    // Traversal
    [[nodiscard]] DynamicArray<T> inorder() const
    {
        DynamicArray<T> result{};

        inorder(root_, result);

        return result;
    }

    [[nodiscard]] DynamicArray<T> preorder() const
    {
        DynamicArray<T> result{};

        preorder(root_, result);

        return result;
    }

    [[nodiscard]] DynamicArray<T> postorder() const
    {
        DynamicArray<T> result{};

        postorder(root_, result);

        return result;
    }

    [[nodiscard]] DynamicArray<T> level_order() const
    {
        DynamicArray<T> result{};

        if (empty())
        {
            return result;
        }

        Queue<const Node*> queue;
        queue.push(root_);

        while(!queue.empty())
        {
            const Node *current{queue.front()};
            result.push_back(current->value);
            queue.pop();

            if (current->left != nullptr)
            {
                queue.push(current->left);
            }

            if(current->right != nullptr)
            {
                queue.push(current->right);
            }

        }

        return result;
    }
};