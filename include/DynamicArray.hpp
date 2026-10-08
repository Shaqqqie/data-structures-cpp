#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

template <typename T>
class DynamicArray
{
public:
    class Iterator
    {
    private:
        T *p{nullptr};

    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::random_access_iterator_tag;

        Iterator() = default;

        explicit Iterator(T *array)
            : p{array}
        {
        }

        T &operator*() const
        {
            return *p;
        }

        Iterator &operator++()
        {
            ++p;

            return *this;
        }

        Iterator operator++(int)
        {
            Iterator old{*this};

            ++(*this);

            return old;
        }

        Iterator &operator--()
        {
            --p;

            return *this;
        }

        Iterator operator--(int)
        {
            Iterator old{*this};

            --(*this);

            return old;
        }

        Iterator operator+(std::ptrdiff_t offset) const
        {
            return Iterator{p + offset};
        }

        Iterator operator-(std::ptrdiff_t offset) const
        {
            return Iterator{p - offset};
        }

        std::ptrdiff_t operator-(const Iterator &other) const
        {
            return p - other.p;
        }

        Iterator &operator+=(std::ptrdiff_t offset)
        {
            p += offset;

            return *this;
        }

        Iterator &operator-=(std::ptrdiff_t offset)
        {
            p -= offset;

            return *this;
        }

        T &operator[](std::ptrdiff_t offset) const
        {
            return p[offset];
        }

        bool operator<(const Iterator &other) const
        {
            return p < other.p;
        }

        bool operator>(const Iterator &other) const
        {
            return p > other.p;
        }

        bool operator<=(const Iterator &other) const
        {
            return p <= other.p;
        }

        bool operator>=(const Iterator &other) const
        {
            return p >= other.p;
        }

        bool operator==(const Iterator &other) const
        {
            return p == other.p;
        }

        bool operator!=(const Iterator &other) const
        {
            return p != other.p;
        }

        friend Iterator operator+(std::ptrdiff_t offset, const Iterator &it)
        {
            return Iterator{it.p + offset};
        }
    };

    class ConstIterator
    {
    private:
        const T *p{nullptr};

    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;
        using iterator_category = std::random_access_iterator_tag;

        ConstIterator() = default;

        explicit ConstIterator(const T *array)
            : p{array}
        {
        }

        const T &operator*() const
        {
            return *p;
        }

        ConstIterator &operator++()
        {
            ++p;

            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator old{*this};

            ++(*this);

            return old;
        }

        ConstIterator &operator--()
        {
            --p;

            return *this;
        }

        ConstIterator operator--(int)
        {
            ConstIterator old{*this};

            --(*this);

            return old;
        }

        ConstIterator operator+(std::ptrdiff_t offset) const
        {
            return ConstIterator{p + offset};
        }

        ConstIterator operator-(std::ptrdiff_t offset) const
        {
            return ConstIterator{p - offset};
        }

        std::ptrdiff_t operator-(const ConstIterator &other) const
        {
            return p - other.p;
        }

        ConstIterator &operator+=(std::ptrdiff_t offset)
        {
            p += offset;

            return *this;
        }

        ConstIterator &operator-=(std::ptrdiff_t offset)
        {
            p -= offset;

            return *this;
        }

        const T &operator[](std::ptrdiff_t offset) const
        {
            return p[offset];
        }

        bool operator<(const ConstIterator &other) const
        {
            return p < other.p;
        }

        bool operator>(const ConstIterator &other) const
        {
            return p > other.p;
        }

        bool operator<=(const ConstIterator &other) const
        {
            return p <= other.p;
        }

        bool operator>=(const ConstIterator &other) const
        {
            return p >= other.p;
        }

        bool operator==(const ConstIterator &other) const
        {
            return p == other.p;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return p != other.p;
        }

        friend ConstIterator operator+(std::ptrdiff_t offset, const ConstIterator &it)
        {
            return ConstIterator{it.p + offset};
        }
    };

private:
    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void reallocate(std::size_t new_capacity)
    {
        T *new_array = new T[new_capacity];

        for (std::size_t i{0}; i < size_; ++i)
        {
            new_array[i] = std::move(data_[i]);
        }

        delete[] data_;

        data_ = new_array;
        capacity_ = new_capacity;
    }

public:
    // Construction / Ownership
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

    DynamicArray(std::initializer_list<T> values)
        : data_{nullptr}, size_{values.size()}, capacity_{values.size()}
    {
        data_ = new T[capacity_];

        T *it = data_;
        for (const T &value : values)
        {
            *it = value;
            ++it;
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
            try
            {
                new_array = new T[other.capacity_];

                for (std::size_t i{0}; i < other.size_; ++i)
                {
                    new_array[i] = other.data_[i];
                }
            }
            catch (...)
            {
                delete[] new_array;
                throw;
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

    bool operator==(const DynamicArray &other) const
    {
        if (size_ != other.size_)
        {
            return false;
        }

        for (std::size_t i{0}; i < size_; ++i)
        {
            if (data_[i] != other.data_[i])
            {
                return false;
            }
        }

        return true;
    }

    ~DynamicArray()
    {
        delete[] data_;
    }

    // Capacity
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

    void reserve(std::size_t new_capacity)
    {
        if (new_capacity <= capacity_)
        {
            return;
        }

        reallocate(new_capacity);
    }

    void resize(std::size_t new_size)
    {
        if (new_size > capacity_)
        {
            reallocate(new_size);
        }

        for (std::size_t i{size_}; i < new_size; ++i)
        {
            data_[i] = T{};
        }

        size_ = new_size;
    }

    void shrink_to_fit()
    {
        if (size_ == capacity_)
        {
            return;
        }

        if (size_ == 0)
        {
            delete[] data_;

            data_ = nullptr;
            capacity_ = 0;

            return;
        }

        reallocate(size_);
    }

    // Element access
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

    T &front()
    {
        if (empty())
        {
            throw std::out_of_range("Empty array.");
        }

        return data_[0];
    }

    const T &front() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty array.");
        }

        return data_[0];
    }

    T &back()
    {
        if (empty())
        {
            throw std::out_of_range("Empty array.");
        }

        return data_[size_ - 1];
    }

    const T &back() const
    {
        if (empty())
        {
            throw std::out_of_range("Empty array.");
        }
        return data_[size_ - 1];
    }

    T *data()
    {
        return data_;
    }

    const T *data() const
    {
        return data_;
    }

    // Modifiers
    void push_back(const T &value)
    {
        if (size_ == capacity_)
        {
            if (capacity_ == 0)
            {
                reallocate(1);
            }
            else
            {
                reallocate(capacity_ * 2);
            }
        }

        data_[size_] = value;
        ++size_;
    }

    void push_back(T &&value)
    {
        if (size_ == capacity_)
        {
            if (capacity_ == 0)
            {
                reallocate(1);
            }
            else
            {
                reallocate(capacity_ * 2);
            }
        }

        data_[size_] = std::move(value);
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

    void erase(std::size_t index)
    {
        if (index >= size_)
        {
            throw std::out_of_range("Invalid index.");
        }

        for (std::size_t i{index}; i + 1 < size_; ++i)
        {
            data_[i] = std::move(data_[i + 1]);
        }

        --size_;
    }

    // Iterators
    Iterator begin()
    {
        return Iterator{data_};
    }

    Iterator end()
    {
        return Iterator{data_ + size_};
    }

    ConstIterator begin() const
    {
        return ConstIterator{data_};
    }

    ConstIterator end() const
    {
        return ConstIterator{data_ + size_};
    }
};
