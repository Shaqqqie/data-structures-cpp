#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"

#include <cstddef>
#include <stdexcept>

template <typename Key, typename Value>
class HashTable
{
private:
    struct Entry
    {
        Key key;
        Value value;
    };

    DynamicArray<LinkedList<Entry>> buckets_;
    std::size_t size_;

public:
    explicit HashTable(std::size_t bucket_count = 8)
        : buckets_{}, size_ { 0 }
    {
        if (bucket_count == 0)
        {
            throw std::invalid_argument("HashTable must have buckets.");
        }

        buckets_.reserve(bucket_count);

        while (buckets_.size() < bucket_count)
        {
            buckets_.push_back(LinkedList<Entry>{});
        }
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return size_ == 0;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] std::size_t bucket_count() const noexcept
    {
        return buckets_.size();
    }
};