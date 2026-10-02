#pragma once

#include "DynamicArray.hpp"

#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class Stack
{
private:
    DynamicArray<T> data_;

public:
    // Construction
    Stack() = default;

    // Capacity / state
    [[nodiscard]] bool empty() const noexcept
    {
        return data_.empty();
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return data_.size();
    }

    // Element access
    T &top()
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty stack.");
        }

        return data_.back();
    }

    const T &top() const
    {
        if (data_.empty())
        {
            throw std::out_of_range("Empty stack.");
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
            throw std::out_of_range("Empty stack.");
        }

        data_.pop_back();
    }
};