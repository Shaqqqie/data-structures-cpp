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
    std::size_t size;

public:
    LinkedList()
        : head{nullptr}, size{0} {}
    
    bool empty() const
    {
        return size == 0;
    }
    
    std::size_t getSize() const
    {
        return size;
    }

    void push_front(const T& value)
    {
        Node* new_node = new Node{value, head};
        head = new_node;
        ++size;
    }

    const T& front() const
    {
        if (!head)
        {
            throw std::out_of_range("Empty list.");
        }

        return head->value;
    }
};