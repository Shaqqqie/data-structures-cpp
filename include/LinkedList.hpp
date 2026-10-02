#pragma once

#include <cstddef>
#include <optional>
#include <stdexcept>

template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T value;
        Node *next;
    };

public:
    class Iterator
    {
    private:
        Node *current;

    public:
        explicit Iterator(Node *node)
            : current{node}
        {
        }

        T &operator*() const
        {
            return current->value;
        }

        Iterator &operator++()
        {
            current = current->next;
            return *this;
        }

        bool operator==(const Iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    class ConstIterator
    {
    private:
        const Node *current;

    public:
        explicit ConstIterator(const Node *node)
            : current{node}
        {
        }

        const T &operator*() const
        {
            return current->value;
        }

        ConstIterator &operator++()
        {
            current = current->next;
            return *this;
        }

        bool operator==(const ConstIterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return current != other.current;
        }
    };

private:
    Node *head;
    Node *tail;
    std::size_t size;

public:
    // Construction / Ownership
    LinkedList()
        : head{nullptr}, tail{nullptr}, size{0}
    {
    }

    LinkedList(const LinkedList &other)
        : head{nullptr}, tail{nullptr}, size{0}
    {
        Node *current{other.head};

        while (current)
        {
            push_back(current->value);
            current = current->next;
        }
    }

    LinkedList(LinkedList &&other) noexcept
        : head{other.head}, tail{other.tail}, size{other.size}
    {
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }
    LinkedList &operator=(const LinkedList &other)
    {
        if (this == &other)
        {
            return *this;
        }

        clear();

        const Node *current{other.head};

        while (current)
        {
            push_back(current->value);
            current = current->next;
        }

        return *this;
    }

    LinkedList &operator=(LinkedList &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        clear();

        head = other.head;
        tail = other.tail;
        size = other.size;

        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;

        return *this;
    }

    ~LinkedList()
    {
        clear();
    }

    // Capacity
    bool empty() const
    {
        return size == 0;
    }

    std::size_t getSize() const
    {
        return size;
    }

    // Element access
    T &at(std::size_t index)
    {
        if (index >= size)
        {
            throw std::out_of_range("Invalid index.");
        }

        Node *current{head};
        std::size_t count{};

        while(count != index)
        {
            current = current->next;
            ++count;
        }

        return current->value;
    }

    const T &at(std::size_t index) const
    {
        if (index >= size)
        {
            throw std::out_of_range("Invalid index.");
        }

        const Node *current{head};
        std::size_t count{};

        while (count != index)
        {
            current = current->next;
            ++count;
        }

        return current->value;
    }

    T &front()
    {
        if (!head)
        {
            throw std::out_of_range("Empty list.");
        }

        return head->value;
    }

    const T &front() const
    {
        if (!head)
        {
            throw std::out_of_range("Empty list.");
        }

        return head->value;
    }

    T &back()
    {
        if (!head)
        {
            throw std::out_of_range("Empty list.");
        }

        return tail->value;
    }
    
    const T &back() const
    {
        if (!tail)
        {
            throw std::out_of_range("Empty list.");
        }

        return tail->value;
    }

    // Modifiers
    void clear()
    {
        Node *current{head};

        while (current)
        {
            Node *next{current->next};
            delete current;
            current = next;
        }

        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    void push_front(const T &value)
    {
        Node *node = new Node{value, head};

        if (empty())
        {
            tail = node;
        }

        head = node;
        ++size;
    }

    void push_back(const T &value)
    {
        Node *node = new Node{value, nullptr};

        if (empty())
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }

        ++size;
    }

    void push_back(T &&value)
    {
        Node *node = new Node{std::move(value), nullptr};

        if (empty())
        {
            head = node;
            tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }

        ++size;
    }

    void pop_front()
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        Node *old_head{head};
        head = head->next;
        delete old_head;
        --size;

        if (empty())
        {
            tail = nullptr;
        }
    }

    void pop_back()
    {
        if (empty())
        {
            throw std::out_of_range("Empty list.");
        }

        if (size == 1)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
            size = 0;
            return;
        }

        Node *current{head};

        while (current->next != tail)
        {
            current = current->next;
        }

        delete tail;
        tail = current;
        tail->next = nullptr;
        --size;
    }

    void insert(std::size_t index, const T &value)
    {
        if (index > size)
        {
            throw std::out_of_range("Invalid index.");
        }

        if (index == 0)
        {
            push_front(value);
            return;
        }

        if (index == size)
        {
            push_back(value);
            return;
        }

        Node *current{head};

        for (std::size_t count{}; count < index - 1; ++count)
        {
            current = current->next;
        }

        Node *node = new Node(value, current->next);
        current->next = node;
        ++size;
    }

    void erase(std::size_t index)
    {
        if (index >= size)
        {
            throw std::out_of_range("Invalid index.");
        }

        if (index == 0)
        {
            pop_front();
            return;
        }

        if (index == size - 1)
        {
            pop_back();
            return;
        }

        Node *current{head};

        for (std::size_t count{}; count < index - 1; ++count)
        {
            current = current->next;
        }

        Node *node{current->next};
        current->next = node->next;
        delete node;
        --size;
    }

    // Search
    bool contains(const T &value) const
    {
        const Node *current{head};

        while (current)
        {
            if (current->value == value)
            {
                return true;
            }

            current = current->next;
        }

        return false;
    }

    std::optional<std::size_t> find(const T &value) const
    {
        std::size_t index{};
        const Node *current{head};

        while (current)
        {
            if (current->value == value)
            {
                return index;
            }

            current = current->next;
            ++index;
        }

        return std::nullopt;
    }

    // Iterators
    Iterator begin()
    {
        return Iterator{head};
    }

    Iterator end()
    {
        return Iterator{nullptr};
    }

    ConstIterator begin() const
    {
        return ConstIterator{head};
    }

    ConstIterator end() const
    {
        return ConstIterator{nullptr};
    }
};
