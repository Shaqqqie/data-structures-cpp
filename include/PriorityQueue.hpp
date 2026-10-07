#pragma once

#include "BinaryHeap.hpp"

#include <cstddef>
#include <functional>
#include <utility>

template <typename T, typename Compare = std::greater<T>>
class PriorityQueue
{
private:
    BinaryHeap<T, Compare> data_;

public:
    PriorityQueue() = default;

    [[nodiscard]] bool empty() const noexcept
    {
        return data_.empty();
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return data_.size();
    }

    const T &top() const
    {
        return data_.top();
    }

    void push(const T &value)
    {
        data_.push(value);
    }

    void push(T &&value)
    {
        data_.push(std::move(value));
    }

    void pop()
    {
        data_.pop();
    }
};