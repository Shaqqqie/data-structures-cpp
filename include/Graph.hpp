#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "Queue.hpp"
#include "Stack.hpp"

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <unordered_set>

template <typename T>
class Graph
{
private:
    struct Vertex
    {
        T value;
        LinkedList<T> neighbors;
    };

    struct StackEntry
    {
        T vertex;
        std::optional<T> parent;
    };

    DynamicArray<Vertex> vertices_;

    bool has_cycle_dfs(const T &current, const std::optional<T> &parent, std::unordered_set<T> &visited) const
    {
        visited.insert(current);

        for (const auto &neighbor : neighbors(current))
        {
            if (!visited.contains(neighbor))
            {
                if (has_cycle_dfs(neighbor, current, visited))
                {
                    return true;
                }
            }
            else if (!parent.has_value() || neighbor != parent.value())
            {
                return true;
            }
        }

        return false;
    }

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
        if (!has_edge(value, other_value))
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

    void clear() noexcept
    {
        vertices_.clear();
    }

    // Traversal
    DynamicArray<T> bfs(const T &start) const
    {
        if (!contains(start))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        Queue<T> queue;
        std::unordered_set<T> visited;
        DynamicArray<T> result{};

        queue.push(start);
        visited.insert(start);

        while (!queue.empty())
        {
            auto vertex = queue.front();

            result.push_back(vertex);

            queue.pop();

            for (const auto &neighbor : neighbors(vertex))
            {
                if (!visited.contains(neighbor))
                {
                    queue.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }

        return result;
    }

    DynamicArray<T> dfs(const T &start) const
    {
        if (!contains(start))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        Stack<T> stack;
        std::unordered_set<T> visited;
        DynamicArray<T> result{};

        stack.push(start);
        visited.insert(start);

        while (!stack.empty())
        {
            auto vertex = stack.top();

            result.push_back(vertex);

            stack.pop();

            for (const auto &neighbor : neighbors(vertex))
            {
                if (!visited.contains(neighbor))
                {
                    stack.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }

        return result;
    }

    [[nodiscard]] bool has_cycle() const
    {
        std::unordered_set<T> visited;

        for (const auto &vertex : vertices_)
        {
            if (!visited.contains(vertex.value))
            {
                if (has_cycle_dfs(vertex.value, std::nullopt, visited))
                {
                    return true;
                }
            }
        }

        return false;
    }

    [[nodiscard]] std::size_t connected_components() const
    {
        std::unordered_set<T> visited;
        std::size_t count{};

        for (const auto &vertex : vertices_)
        {
            if (!visited.contains(vertex.value))
            {
                ++count;

                const DynamicArray<T> connected_vertices{dfs(vertex.value)};

                for (const auto &connected_vertex : connected_vertices)
                {
                    visited.insert(connected_vertex);
                }
            }
        }

        return count;
    }
};