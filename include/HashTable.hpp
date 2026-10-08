#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"

#include <cstddef>
#include <functional>
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

    std::size_t bucket_index(const Key &key) const
    {
        return std::hash<Key>{}(key) % bucket_count();
    }

public:
    // Construction / Ownership
    explicit HashTable(std::size_t bucket_count = 8)
        : buckets_{}, size_{0}
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

    // Capacity / State
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

    // Modifiers
    bool insert(const Key &key, const Value &value)
    {
        std::size_t index{bucket_index(key)};

        auto &bucket = buckets_[index];

        for (const auto &entry : bucket)
        {
            if (key == entry.key)
            {
                return false;
            }
        }

        bucket.push_back(Entry{key, value});
        ++size_;

        return true;
    }

    // Element Access
    [[nodiscard]] bool contains(const Key &key) const
    {
        std::size_t index{bucket_index(key)};

        const auto &bucket = buckets_[index];

        for (const auto &entry : bucket)
        {
            if (key == entry.key)
            {
                return true;
            }
        }

        return false;
    }

    Value &at(const Key &key)
    {
        const std::size_t index{bucket_index(key)};

        auto &bucket = buckets_[index];

        for (auto &entry : bucket)
        {
            if (key == entry.key)
            {
                return entry.value;
            }
        }

        throw std::out_of_range("No matching key exists.");
    }

    const Value &at(const Key &key) const
    {
        const std::size_t index{bucket_index(key)};

        const auto &bucket = buckets_[index];

        for (const auto &entry : bucket)
        {
            if (key == entry.key)
            {
                return entry.value;
            }
        }

        throw std::out_of_range("No matching key exists.");
    }
};