#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "Queue.hpp"
#include "Stack.hpp"

#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

template <typename T>
class DirectedGraph
{
private:
    struct Vertex
    {
        T value;
        LinkedList<T> neighbors;
    };

    DynamicArray<Vertex> vertices_;

public:
    DirectedGraph() = default;

    //------------------------------------------------------------------------//
    // Capacity / State
    //-----------------------------------------------------------------------//
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

        return count;
    }

    //------------------------------------------------------------------------//
    // Element Access
    //-----------------------------------------------------------------------//
    [[nodiscard]] bool contains_vertex(const T &value) const
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

    [[nodiscard]] bool contains_edge(const T &from, const T &to) const
    {
        for (const auto &vertex : vertices_)
        {
            if (from == vertex.value)
            {
                return vertex.neighbors.contains(to);
            }
        }

        return false;
    }

    [[nodiscard]] DynamicArray<T> neighbors(const T &value) const
    {
        for (const auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                DynamicArray<T> result;

                for (const auto &neighbor : vertex.neighbors)
                {
                    result.push_back(neighbor);
                }
                
                return result;
            }
        }

        throw std::out_of_range("Vertex does not exist.");
    }

    //------------------------------------------------------------------------//
    // Modifiers
    //-----------------------------------------------------------------------//
    void add_vertex(const T &value)
    {
        if (contains_vertex(value))
        {
            return;
        }

        Vertex vertex{value, {}};

        vertices_.push_back(vertex);
    }

    void add_edge(const T &value, const T &other_value)
    {
        if (!contains_vertex(value) || !contains_vertex(other_value))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        if (value == other_value)
        {
            throw std::invalid_argument("Cannot have self-loops.");
        }

        if (contains_edge(value, other_value))
        {
            throw std::invalid_argument("Edge already exists.");
        }

        for (auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                vertex.neighbors.push_back(other_value);
                break;
            }
        }
    }

    void remove_edge(const T &from, const T &to)
    {
        if (!contains_vertex(from) || !contains_vertex(to) || !contains_edge(from, to))
        {
            throw std::out_of_range("One vertex or edge between vertices does not exist.");
        }

        for (auto &vertex : vertices_)
        {
            if (from == vertex.value)
            {
                const auto index = vertex.neighbors.find(to);
                vertex.neighbors.erase(*index);
                break;
            }
        }
    }

    void remove_vertex(const T &value)
    {
        if (!contains_vertex(value))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        for (auto &vertex : vertices_)
        {
            const auto index = vertex.neighbors.find(value);
            
            if (index.has_value())
            {
                vertex.neighbors.erase(*index);
            }
        }

        std::size_t index{};
        for(auto &vertex : vertices_)
        {
            if (value == vertex.value)
            {
                vertices_.erase(index);
                break;
            }

            ++index;
        }
    }

    //------------------------------------------------------------------------//
    // Traversal
    //-----------------------------------------------------------------------//
    DynamicArray<T> bfs(const T &start) const
    {
        if (!contains_vertex(start))
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
        if (!contains_vertex(start))
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

    [[nodiscard]] DynamicArray<T> topological_sort() const
    {
        std::unordered_map<T, int> in_degree;

        for (const auto &vertex : vertices_)
        {
            in_degree[vertex.value] = 0;
        }

        for (const auto &vertex: vertices_)
        {
            for (const auto &neighbor : vertex.neighbors)
            {
                in_degree.at(neighbor) += 1;
            }
        }

        Queue<T> queue;

        for (const auto &vertex : vertices_)
        {
            if (in_degree.at(vertex.value) == 0)
            {
                queue.push(vertex.value);
            }
        }

        DynamicArray<T> result{};

        while (!queue.empty())
        {
            auto vertex_value = queue.front();
            queue.pop();

            result.push_back(vertex_value);

            for (const auto &vertex : vertices_)
            {
                if (vertex_value == vertex.value)
                {
                    for (const auto &neighbor : vertex.neighbors)
                    {
                        in_degree.at(neighbor) -= 1;
                        
                        if (in_degree.at(neighbor) == 0)
                        {
                            queue.push(neighbor);
                        }
                    }

                    break;
                }
            }
        }

        if (result.size() != vertex_count())
        {
            throw std::logic_error(
                "Cannot topologically sort a graph containing a cycle."
            );
        }

        return result;
    }
};