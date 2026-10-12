#pragma once

#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "PriorityQueue.hpp"

#include <cstddef>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <utility>

template <typename T>
class WeightedGraph
{
private:
    struct DijkstraResult
    {
        std::unordered_map<T, double> distances;
        std::unordered_map<T, T> parents;
    };

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

    [[nodiscard]] DijkstraResult dijkstra(const T &start) const
    {
        DijkstraResult result;
        result.distances[start] = 0.0;
        using Entry = std::pair<double, T>;
        PriorityQueue<Entry, std::less<Entry>> queue;
        queue.push({0.0, start});

        while(!queue.empty())
        {
            const auto [current_distance, current_vertex] = queue.top();
            queue.pop();

            if (current_distance > result.distances.at(current_vertex))
            {
                continue;
            }

            for (const auto &vertex : vertices_)
            {
                if (current_vertex == vertex.value)
                {
                    for (const auto &edge : vertex.neighbors)
                    {
                        double new_distance = current_distance + edge.weight;

                        auto it = result.distances.find(edge.destination);
                        if (it == result.distances.end() || new_distance < it->second)
                        {
                            result.distances[edge.destination] = new_distance;
                            result.parents[edge.destination] = current_vertex;
                            queue.push({new_distance, edge.destination});
                        }
                    }

                    break;
                }
            }
        }

        return result;
    }

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

    // Traversal
    [[nodiscard]] double shortest_distance(const T &start, const T &end) const
    {
        if (!contains(start) || !contains(end))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        DijkstraResult result = dijkstra(start);

        auto it = result.distances.find(end);

        if (it == result.distances.end())
        {
            return std::numeric_limits<double>::infinity();
        }

      
        return it->second;
    }

    [[nodiscard]] DynamicArray<T> shortest_path(const T &start, const T &end) const
    {
        if (!contains(start) || !contains(end))
        {
            throw std::out_of_range("Vertex does not exist.");
        }

        DijkstraResult result = dijkstra(start);
        DynamicArray<T> path;

        if (!result.distances.contains(end))
        {
            return {};
        }

        T current = end;

        while(current != start)
        {
            path.push_back(current);
            current = result.parents.at(current);
        }
        path.push_back(start);

        std::reverse(path.begin(), path.end());

        return path;
    }
};