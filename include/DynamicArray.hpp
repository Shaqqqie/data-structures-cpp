#pragma once

#include <cstddef>
#include <stdexcept>

template <typename T>
class DynamicArray
{
private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void resize()
    {
        if (capacity_ == 0)
        {
            capacity_ = 1;
        }
        else
        {
            capacity_ *= 2;
        }

        T *new_array = new T[capacity_];

        for (std::size_t i{0}; i < size_; ++i)
        {
            new_array[i] = data_[i];
        }

        delete[] data_;
        data_ = new_array;
    }

public:
    DynamicArray()
        : data_{nullptr}, size_{0}, capacity_{0}
    {
    }

    DynamicArray(const DynamicArray &other)
        : data_{nullptr}, size_{other.size_}, capacity_{other.capacity_}
    {
        if (other.capacity_ > 0)
        {
            data_ = new T[other.capacity_];

            for (std::size_t i{0}; i < other.size_; ++i)
            {
                data_[i] = other.data_[i];
            }
        }
    }

    DynamicArray(DynamicArray &&other) noexcept
    : data_{other.data_}, size_{other.size_}, capacity_{other.capacity_}
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this == &other)
        {
            return *this;
        }

        T *new_array{nullptr};

        if (other.capacity_ > 0)
        {
            new_array = new T[other.capacity_];

            for (std::size_t i{0}; i < other.size_; ++i)
            {
                new_array[i] = other.data_[i];
            }
        }

        delete[] data_;

        data_ = new_array;
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    DynamicArray &operator=(DynamicArray &&other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
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

    T &at(std::size_t index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("Invalid index.");
        }

        return data_[index];
    }

    const T &at(std::size_t index) const
    {
        if (index >= size_)
        {
            throw std::out_of_range("Invalid index.");
        }

        return data_[index];
    }

    T &operator[](std::size_t index)
    {
        return data_[index];
    }
    const T &operator[](std::size_t index) const
    {
        return data_[index];
    }

    void push_back(const T &value)
    {
        if (size_ == capacity_)
        {
            resize();
        }

        data_[size_] = value;
        ++size_;
    }

    void pop_back()
    {
        if (size_ > 0)
        {
            --size_;
        }
    }

    void clear()
    {
        size_ = 0;
    }
};
