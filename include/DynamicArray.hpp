#pragma once

#include <cstddef>

template <typename T>
class DynamicArray
{
private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void resize();

public:
    DynamicArray()
    : data_{nullptr}, size_{0}, capacity_{0}
    {
    }
    ~DynamicArray()
    {
        delete[] data_;
    }

    [[nodiscard]] std::size_t size() const
    {
        return size_;
    }

    [[nodiscard]] std::size_t capacity() const
    {
        return capacity_;
    }

    [[nodiscard]] bool empty() const
    {
        return size_ == 0;
    }

    T &at(std::size_t index);
    const T &at(std::size_t index) const;

    T &operator[](std::size_t index);
    const T &operator[](std::size_t index) const;

    void push_back(const T &value);
    void pop_back();
    void clear();
};