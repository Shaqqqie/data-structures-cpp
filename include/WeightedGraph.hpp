#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"

#include <cstddef>
#include <cmath>
#include <stdexcept>

template <typename T>
class WeightedGraph
{
private:
    struct Edge
    {
        T destination;
        double weight;
    };

    struct Vertex
    {
        T value;
        LinkedList<Edge> neighbors;
    };

    DynamicArray<Vertex> vertices_;

public:
    // Construction
    WeightedGraph() = default;

    // Capacity / State
    [[nodiscard]] bool empty() const noexcept
    {
        return vertices_.empty();
    }

    [[nodiscard]] std::size_t vertex_count() const noexcept
    {
        return vertices_.size();
    }

    [[nodiscard]] std::size_t edge_count() const noexcept
    {
        std::size_t count{};
        for (const auto &vertex : vertices_)
        {
            count += vertex.neighbors.getSize();
        }

        return count / 2;
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
                    if (neighbor.destination == other_value)
                    {
                        return true;
                    }
                }

                return false;
            }
        }

        return false;
    }

    [[nodiscard]] double weight(const T &value, const T &other_value) const
    {
        if (!contains(value) || !contains(other_value))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        for (const auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                for (const auto &neighbor : vertex.neighbors)
                {
                    if (neighbor.destination == other_value)
                    {
                        return neighbor.weight;
                    }
                }

                break;
            }
        }

        throw std::out_of_range("Edge does not exist.");
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

        for (auto &vertex : vertices_)
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

    void add_edge(const T &value, const T &other_value, double weight)
    {
        if (!std::isfinite(weight) || weight < 0.0)
        {
            throw std::invalid_argument("Edge weight must be finite and nonnegative.");
        }

        if (!contains(value) || !contains(other_value) || value == other_value)
        {
            return;
        }

        if (has_edge(value, other_value))
        {
            for (auto &vertex : vertices_)
            {
                if (value == vertex.value)
                {
                    for (auto &neighbor : vertex.neighbors)
                    {
                        if (neighbor.destination == other_value)
                        {
                            neighbor.weight = weight;
                        }
                    }
                }

                if (other_value == vertex.value)
                {
                    for (auto &neighbor : vertex.neighbors)
                    {
                        if (neighbor.destination == value)
                        {
                            neighbor.weight = weight;
                        }
                    }
                }
            }

            return;
        }

        Vertex *first{nullptr};
        Vertex *second{nullptr};
        for (auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                first = &vertex;
            }

            if (other_value == vertex.value)
            {
                second = &vertex;
            }
        }

        first->neighbors.push_back(Edge{other_value, weight});
        try
        {
            second->neighbors.push_back(Edge{value, weight});
        }
        catch (...)
        {
            first->neighbors.pop_back();
            throw;
        }
    }

    void remove_edge(const T &value, const T &other_value)
    {
        if (!has_edge(value, other_value))
        {
            return;
        }

        for (auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                std::size_t index{};
                for (auto &neighbor : vertex.neighbors)
                {
                    if (neighbor.destination == other_value)
                    {
                        vertex.neighbors.erase(index);
                        break;
                    }

                    ++index;
                }
            }

            if (other_value == vertex.value)
            {
                std::size_t index{};
                for (auto &neighbor : vertex.neighbors)
                {
                    if (neighbor.destination == value)
                    {
                        vertex.neighbors.erase(index);
                        break;
                    }

                    ++index;
                }
            }
        }
    }

    void clear() noexcept
    {
        vertices_.clear();
    }
};