#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class DoublyLinkedList
{
private:
    struct Node
    {
        T value;
        Node *previous;
        Node *next;
    };

    Node *head_;
    Node *tail_;
    std::size_t size_;

public:
    // Construction / Ownership
    DoublyLinkedList()
        : head_{nullptr}, tail_{nullptr}, size_{0}
    {
    }

    DoublyLinkedList(const DoublyLinkedList &other)
        : head_{nullptr}, tail_{nullptr}, size_{0}
    {
        try
        {
            Node *current{other.head_};

            while (current)
            {
                push_back(current->value);
                current = current->next;
            }
        }
        catch (...)
        {
            clear();
            throw;
        }
    }

    DoublyLinkedList(DoublyLinkedList &&other) noexcept
        : head_{other.head_}, tail_{other.tail_}, size_{other.size_}
    {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    DoublyLinkedList &operator=(const DoublyLinkedList &other)
    {
        if (this == &other)
        {
            return *this;
        }

        DoublyLinkedList temp{other};
        *this = std::move(temp);

        return *this;
    }

    DoublyLinkedList &operator=(DoublyLinkedList &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        clear();

        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;

        return *this;
    }

    ~DoublyLinkedList()
    {
        clear();
    }

    // Capacity
    [[nodiscard]] bool empty() const noexcept
    {
        return size_ == 0;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return size_;
    }

    // Element access
    T &front()
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        return head_->value;
    }

    const T &front() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        return head_->value;
    }

    T &back()
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        return tail_->value;
    }

    const T &back() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        return tail_->value;
    }

    // Modifiers
    void clear() noexcept
    {
        Node *current{head_};

        while (current)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }

        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    void push_front(const T &value)
    {
        Node *node = new Node{value, nullptr, head_};

        if (empty())
        {
            head_ = node;
            tail_ = node;
        }
        else
        {
            head_->previous = node;
            head_ = node;
        }

        ++size_;
    }

    void push_front(T &&value)
    {
        Node *node = new Node{std::move(value), nullptr, head_};

        if (empty())
        {
            head_ = node;
            tail_ = node;
        }
        else
        {
            head_->previous = node;
            head_ = node;
        }

        ++size_;
    }

    void push_back(const T &value)
    {
        Node *node = new Node{value, tail_, nullptr};

        if (empty())
        {
            tail_ = node;
            head_ = node;
        }
        else
        {
            tail_->next = node;
            tail_ = node;
        }

        ++size_;
    }

    void push_back(T &&value)
    {
        Node *node = new Node{std::move(value), tail_, nullptr};

        if (empty())
        {
            tail_ = node;
            head_ = node;
        }
        else
        {
            tail_->next = node;
            tail_ = node;
        }

        ++size_;
    }

    void pop_front()
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        Node *front{head_};
        head_ = head_->next;

        if (head_ == nullptr)
        {
            tail_ = nullptr;
        }
        else
        {
            head_->previous = nullptr;
        }

        delete front;
        --size_;
    }

    void pop_back()
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        Node *back{tail_};

        tail_ = tail_->previous;

        if (tail_ == nullptr)
        {
            head_ = nullptr;
        }
        else
        {
            tail_->next = nullptr;
        }

        delete back;
        --size_;
    }
};