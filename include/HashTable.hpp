#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"

#include <cstddef>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <utility>

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
    float max_load_factor_{0.75f};

    std::size_t bucket_index(const Key &key) const
    {
        return std::hash<Key>{}(key) % bucket_count();
    }

    void rehash(std::size_t new_bucket_count)
    {
        if (new_bucket_count == 0)
        {
            throw std::invalid_argument("HashTable must have buckets.");
        }

        // Create new array and construct empty lists
        DynamicArray<LinkedList<Entry>> new_buckets_;
        new_buckets_.reserve(new_bucket_count);
        for (std::size_t i{0}; i < new_bucket_count; ++i)
        {
            new_buckets_.push_back(LinkedList<Entry>{});
        }

        // Calculate new index of every entry for the new array,
        // and push entry into new array
        for (const auto &bucket : buckets_)
        {
            for (const auto &entry : bucket)
            {
                std::size_t index{std::hash<Key>{}(entry.key) % new_bucket_count};
                new_buckets_[index].push_back(entry);
            }
        }

        // Move new array into old array
        buckets_ = std::move(new_buckets_);
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

    HashTable(const HashTable &other) = default;

    HashTable &operator=(const HashTable &other) = default;

    HashTable(HashTable &&other) noexcept
        : buckets_{std::move(other.buckets_)}, size_{other.size_}, max_load_factor_{other.max_load_factor_}
    {
        other.size_ = 0;
    }

    HashTable &operator=(HashTable &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        buckets_ = std::move(other.buckets_);
        size_ = std::move(other.size_);
        max_load_factor_ = std::move(other.max_load_factor_);

        other.size_ = 0;

        return *this;
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

    [[nodiscard]] float load_factor() const noexcept
    {
        if (bucket_count() == 0)
        {
            return 0.0f;
        }

        return static_cast<float>(size_) / bucket_count();
    }

    [[nodiscard]] float max_load_factor() const noexcept
    {
        return max_load_factor_;
    }

    // Modifiers
    bool insert(const Key &key, const Value &value)
    {
        if (bucket_count() == 0)
        {
            rehash(8);
        }

        std::size_t index{bucket_index(key)};

        // Duplicate key check
        for (const auto &entry : buckets_[index])
        {
            if (key == entry.key)
            {
                return false;
            }
        }

        // Predict load factor after insertion
        auto load_factor_after_insert = static_cast<float>(size_ + 1) / bucket_count();

        // Rehash if necessary
        if (load_factor_after_insert > max_load_factor())
        {
            rehash(bucket_count() * 2);
            index = bucket_index(key);
        }

        // Insert into correct bucket
        buckets_[index].push_back(Entry{key, value});
        ++size_;

        return true;
    }

    bool erase(const Key &key)
    {
        if (bucket_count() == 0)
        {
            return false;
        }

        const std::size_t index{bucket_index(key)};

        auto &bucket = buckets_[index];

        std::size_t list_index{};

        for (const auto &entry : bucket)
        {
            if (key == entry.key)
            {
                bucket.erase(list_index);
                --size_;
                return true;
            }

            ++list_index;
        }

        return false;
    }

    void clear()
    {
        for (auto &bucket : buckets_)
        {
            bucket.clear();
        }

        size_ = 0;
    }

    // Element Access
    [[nodiscard]] bool contains(const Key &key) const
    {
        if (empty())
        {
            return false;
        }

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
        if (empty())
        {
            throw std::out_of_range("No buckets present.");
        }

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
        if (empty())
        {
            throw std::out_of_range("No buckets present.");
        }

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