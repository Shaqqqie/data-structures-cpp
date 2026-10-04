#pragma once

#include "DoublyLinkedList.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class Deque
{
    private:
    DoublyLinkedList<T> data_;

    public:
    // Construction
    Deque() = default;

    // Capacity / State
    [[nodiscard]] bool empty() const noexcept
    {
        return data_.empty();
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return data_.size();
    }

    // Element Access
    T &front() 
    {
        if (empty())
        {
            throw std::out_of_range("Empty deque.");
        }

        return data_.front();
    }

    const T &front() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty deque.");
        }

        return data_.front();
    }

    T &back()
    {
        if (empty())
        {
            throw std::out_of_range("Empty deque.");
        }

        return data_.back();
    }

    const T &back() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty deque.");
        }

        return data_.back();
    }

    // Modifiers
    void push_front(const T &value)
    {
        data_.push_front(value);
    }

    void push_front(T &&value)
    {
        data_.push_front(std::move(value));
    }

    void push_back(const T &value)
    {
        data_.push_back(value);
    }

    void push_back(T &&value)
    {
        data_.push_back(std::move(value));
    }

    void pop_front()
    {
        if (empty())
        {
            throw std::out_of_range("Empty deque.");
        }

        data_.pop_front();
    }

    void pop_back()
    {
        if (empty())
        {
            throw std::out_of_range("Empty deque.");
        }

        data_.pop_back();
    }
};