#pragma once

#include <cstddef>
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

    Node *head;
    Node *tail;
    std::size_t size;

public:
    // Construction / Ownership
    LinkedList()
        : head{nullptr}, tail{nullptr}, size{0} {}

    LinkedList(const LinkedList &other)
        : head{nullptr}, tail{nullptr}, size{0}
    {
        Node* current{other.head};

        while(current)
        {
            push_back(current->value);
            current = current->next;
        }
    }

    ~LinkedList()
    {
       clear();
    }

    LinkedList& operator=(const LinkedList& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        clear();

        Node* current{other.head};

        while(current)
        {
            push_back(current->value);
            current = current->next;
        }

        return *this;
    }

    void clear()
    {
        Node* current{head};

        while(current)
        {
            Node* next = current->next;
            delete current;
            current = next;
        }

        head = tail = nullptr;
        size = 0;
    }

    // Access
    bool empty() const
    {
        return size == 0;
    }

    std::size_t getSize() const
    {
        return size;
    }

    const T& front() const
    {
        if (!head)
        {
            throw std::out_of_range("Empty list.");
        }

        return head->value;
    }

    const T& back() const
    {
        if (!tail)
        {
            throw std::out_of_range("Empty list.");
        }

        return tail->value;
    }

    // Insertion
    void push_front(const T &value)
    {
        Node *node = new Node{value, head};
        if (size == 0)
        {
            tail = node;
        }

        head = node;

        ++size;
    }

    void push_back(const T &value)
    {
        Node *node = new Node{value, nullptr};

        if (!head)
        {
            head = tail = node;
        }
        else
        {
            tail->next = node;
            tail = node;
        }

        ++size;
    }

    // Removal
    void pop_front()
    {
        if (!head)
        {
            throw std::out_of_range("Empty list.");
        }

        Node* front{head};
        head = head->next;
        delete front;
        --size;

        if (size == 0)
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
            Node* node{head};
            head = tail = nullptr;
            delete node;
            size = 0;

            return;
        }

        Node* current{head};

        while(current->next != tail)
        {
            current = current->next;
        }

        Node* back{current->next};
        tail = current;
        tail->next = nullptr;
        delete back;

        --size;
    }


    
};