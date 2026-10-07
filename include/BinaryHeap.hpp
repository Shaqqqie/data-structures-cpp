#pragma once

#include "DynamicArray.hpp"

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

template <typename T, typename Compare = std::less<T>>
class BinaryHeap
{
private:
    DynamicArray<T> data_;
    Compare compare_;

    void sift_up(std::size_t index)
    {
        while (index != 0)
        {
            std::size_t parent{(index - 1) / 2};

            if (compare_(data_[index], data_[parent]))
            {
                std::swap(data_[index], data_[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }

    void sift_down(std::size_t index)
    {
        while (true)
        {
            std::size_t left{2 * index + 1};
            if (left >= data_.size())
            {
                break;
            }

            std::size_t higher_priority_child{left};
            std::size_t right{2 * index + 2};

            if (right < data_.size() && compare_(data_[right], data_[left]))
            {
                higher_priority_child = right;
            }

            if (compare_(data_[higher_priority_child], data_[index]))
            {
                std::swap(data_[index], data_[higher_priority_child]);
                index = higher_priority_child;
            }
            else
            {
                break;
            }
        }
    }

public:
    // Construction / Ownership
    BinaryHeap() = default;

    BinaryHeap(const DynamicArray<T> &values)
    : data_{values}
    {
        for(std::size_t i{data_.size() / 2}; i > 0; --i)
        {
            sift_down(i - 1);
        }
    }

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
    const T &top() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty heap.");
        }

        return data_[0];
    }

    // Modifiers
    void push(const T &value)
    {
        data_.push_back(value);

        sift_up(data_.size() - 1);
    }

    void push(T &&value)
    {
        data_.push_back(std::move(value));
        sift_up(data_.size() - 1);
    }

    void pop()
    {
        if (empty())
        {
            throw std::out_of_range("Empty heap.");
        }

        if (size() == 1)
        {
            data_.pop_back();
            return;
        }

        std::swap(data_[0], data_[data_.size() - 1]);
        data_.pop_back();

        sift_down(0);
    }
};