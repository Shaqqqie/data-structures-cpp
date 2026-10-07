#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"

#include <cstddef>
#include <stdexcept>

template <typename T>
class Graph
{
private:
    struct Vertex
    {
        T value;
        LinkedList<T> neighbors;
    };

    DynamicArray<Vertex> vertices_;

public:
    // Construction / Ownership
    Graph() = default;

    // Capacity / State
    [[nodiscard]] bool empty() const noexcept
    {
        return vertices_.empty();
    }

    [[nodiscard]] std::size_t vertex_count() const noexcept
    {
        return vertices_.size();
    }

    // Element Access
    [[nodiscard]] bool contains(const T &value) const
    {
        for (const auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                return true;
            }
        }

        return false;
    }

    [[nodiscard]] bool has_edge(const T &value, const T &other_value) const
    {
        if (!contains(value) || !contains(other_value))
        {
            return false;
        }

        for (const auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                for (const auto &neighbor : vertex.neighbors)
                {
                    if (neighbor == other_value)
                    {
                        return true;
                    }
                }

                return false;
            }
        }

        return false;
    }

    [[nodiscard]] const LinkedList<T> &neighbors(const T &value) const
    {
        if (!contains(value))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        for (const auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                return vertex.neighbors;
            }
        }

        throw std::out_of_range("Vertex does not exist.");
    }

    // Modifiers
    void add_vertex(const T &value)
    {
        if (contains(value))
        {
            return;
        }

        Vertex vertex{value, {}};

        vertices_.push_back(vertex);
    }

    void remove_vertex(const T &value)
    {
        if (!contains(value))
        {
            return;
        }

        for(auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                continue;
            }

            remove_edge(value, vertex.value);
        }

        std::size_t index{};

        for (const auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                break;
            }

            ++index;
        }

        vertices_.erase(index);

    }

    void add_edge(const T &value, const T &other_value)
    {
        if (!contains(value) || !contains(other_value) || value == other_value || has_edge(value, other_value))
        {
            return;
        }

        for (auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                vertex.neighbors.push_back(other_value);
            }

            if (other_value == vertex.value)
            {
                vertex.neighbors.push_back(value);
            }
        }
    }

    void remove_edge(const T &value, const T &other_value)
    {
        if(!has_edge(value, other_value))
        {
            return;
        }

        for (auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                auto index{vertex.neighbors.find(other_value)};
                if (index)
                {
                    vertex.neighbors.erase(*index);
                }
            }

            if (other_value == vertex.value)
            {
                auto index{vertex.neighbors.find(value)};
                
                if (index)
                {
                    vertex.neighbors.erase(*index);
                }
            }
        }
    }
};