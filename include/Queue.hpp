#pragma once

#include "LinkedList.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class Queue
{
private:
    LinkedList<T> data_;

public:
    // Construction
    Queue() = default;

    // Capacity / State
    [[nodiscard]] bool empty() const noexcept
    {
        return data_.empty();
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return data_.getSize();
    }

    // Element access
    T &front()
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty queue.");
        }

        return data_.front();
    }

    const T &front() const
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty queue.");
        }

        return data_.front();
    }

    T &back()
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty queue.");
        }

        return data_.back();
    }

    const T &back() const
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty queue.");
        }

        return data_.back();
    }

    // Modifiers
    void push(const T &value)
    {
        data_.push_back(value);
    }

    void push(T &&value)
    {
        data_.push_back(std::move(value));
    }

    void pop()
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty queue.");
        }

        data_.pop_front();
    }

    void clear()
    {
        data_.clear();
    }
};