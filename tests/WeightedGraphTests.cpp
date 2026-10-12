#include <catch2/catch_test_macros.hpp>

#include "WeightedGraph.hpp"

#include <cmath>
#include <stdexcept>

TEST_CASE("WeightedGraph weight() operations", "[WeightedGraph]")
{
    SECTION("Returns the correct weight for an existing edge")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));
        REQUIRE(graph.weight(10, 20) == 5.0);
        REQUIRE(graph.weight(20, 10) == 5.0);
    }

    SECTION("Works with fractional weights")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 2.5);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));
        REQUIRE(graph.weight(10, 20) == 2.5);
        REQUIRE(graph.weight(20, 10) == 2.5);
    }

    SECTION("Throws when one vertex does not exist")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE_THROWS_AS(graph.weight(10, 30), std::out_of_range);
    }

    SECTION("Throws when both vertices exist but no edge connects them")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_THROWS_AS(graph.weight(10, 20), std::out_of_range);
    }

    SECTION("Updating an existing edge changes its weight in both directions")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(10, 20, 8.0);

        REQUIRE(graph.weight(10, 20) == 8.0);
        REQUIRE(graph.weight(20, 10) == 8.0);
        REQUIRE(graph.edge_count() == 1);
    }

    SECTION("Updating an edge does not affect other edges")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 7.0);

        graph.add_edge(10, 20, 8.0);

        REQUIRE(graph.weight(10, 20) == 8.0);
        REQUIRE(graph.weight(20, 10) == 8.0);

        REQUIRE(graph.weight(20, 30) == 7.0);
        REQUIRE(graph.weight(30, 20) == 7.0);

        REQUIRE(graph.edge_count() == 2);
    }
}

TEST_CASE("WeightedGraph remove_edge() operations", "[WeightedGraph]")
{
    SECTION("Removing an existing edge removes it in both directions")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));

        graph.remove_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE_FALSE(graph.has_edge(20, 10));
    }

    SECTION("Removing an edge decreases edge_count() by one")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.edge_count() == 1);

        graph.remove_edge(10, 20);

        REQUIRE(graph.edge_count() == 0);
    }

    SECTION("Removing a nonexistent edge does nothing")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));
        REQUIRE(graph.edge_count() == 1);

        graph.remove_edge(30, 40);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));
        REQUIRE(graph.edge_count() == 1);
    }

    SECTION("Removing one edge preserves other edges and weights")
    {
        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 7.5);

        graph.remove_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE_FALSE(graph.has_edge(20, 10));

        REQUIRE(graph.has_edge(20, 30));
        REQUIRE(graph.has_edge(30, 20));

        REQUIRE(graph.weight(20, 30) == 7.5);
        REQUIRE(graph.weight(30, 20) == 7.5);

        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.vertex_count() == 3);
    }

    SECTION("Removing the last remaining edge leaves both vertices intact")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.vertex_count() == 2);

        graph.remove_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE(graph.edge_count() == 0);
        REQUIRE(graph.vertex_count() == 2);
    }
}

TEST_CASE("WeightedGraph remove_vertex() operations", "[WeightedGraph]")
{
    SECTION("Removing a leaf vertex leaves all edges intact")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 5.0);
        graph.add_edge(20, 40, 5.0);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 30));
        REQUIRE(graph.has_edge(20, 40));
        REQUIRE(graph.edge_count() == 3);
        REQUIRE(graph.vertex_count() == 4);

        graph.remove_vertex(40);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 30));
        REQUIRE_FALSE(graph.has_edge(20, 40));
        REQUIRE(graph.edge_count() == 2);
        REQUIRE(graph.vertex_count() == 3);
    }

    SECTION("Removing an isolated vertex preserves all edges")
    {
        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);

        graph.remove_vertex(30);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));
        REQUIRE(graph.weight(10, 20) == 5.0);
    }

    SECTION("Removing a vertex connected to several others removes all edges between vertex and others")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 5.0);
        graph.add_edge(20, 40, 5.0);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 30));
        REQUIRE(graph.has_edge(20, 40));
        REQUIRE(graph.edge_count() == 3);
        REQUIRE(graph.vertex_count() == 4);

        graph.remove_vertex(20);

        REQUIRE(graph.edge_count() == 0);
        REQUIRE(graph.vertex_count() == 3);
        REQUIRE_FALSE(graph.contains(20));

        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(30));
        REQUIRE(graph.contains(40));
    }

    SECTION("Removing a vertex from a triangle leaves remaining edges intact")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 5.0);
        graph.add_edge(30, 10, 7.0);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE(graph.edge_count() == 3);
        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 30));
        REQUIRE(graph.has_edge(30, 10));

        graph.remove_vertex(30);

        REQUIRE_FALSE(graph.contains(30));
        REQUIRE_FALSE(graph.has_edge(30, 10));

        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.has_edge(10, 20));
    }

    SECTION("Removing a nonexistent vertex does nothing")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));

        graph.remove_vertex(30);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));
    }

    SECTION("Removing the last remaining vertex leaves graph empty")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE(graph.vertex_count() == 1);
        REQUIRE_FALSE(graph.empty());

        graph.remove_vertex(10);

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
    }
}

TEST_CASE("WeightedGraph clear() operations", "[WeightedGraph]")
{
    SECTION("Clearing an empty graph does nothing")
    {
        WeightedGraph<int> graph;
        graph.clear();

        REQUIRE(graph.empty());
    }

    SECTION("Clearing a graph containing isolated vertices leaves graph empty")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.contains(30));

        graph.clear();

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
        REQUIRE_FALSE(graph.contains(10));
        REQUIRE_FALSE(graph.contains(20));
        REQUIRE_FALSE(graph.contains(30));
    }

    SECTION("Clearing a graph with multiple weighted edges leaves graph empty")
    {
        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 7.5);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE(graph.edge_count() == 2);
        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.contains(30));

        graph.clear();

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
        REQUIRE(graph.edge_count() == 0);
        REQUIRE_FALSE(graph.contains(10));
        REQUIRE_FALSE(graph.contains(20));
        REQUIRE_FALSE(graph.contains(30));
    }

    SECTION("Reusing graph after clearing is possible")
    {
        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 7.5);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE(graph.edge_count() == 2);
        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.contains(30));

        graph.clear();

        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(40, 50, 10.0);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.contains(40));
        REQUIRE(graph.contains(50));

        REQUIRE(graph.has_edge(40, 50));
        REQUIRE(graph.has_edge(50, 40));
        REQUIRE(graph.weight(40, 50) == 10.0);
        REQUIRE(graph.weight(50, 40) == 10.0);
    }
}

TEST_CASE("Dijkstra tests", "[WeightedGraph]")
{
    SECTION("Throws for missing start vertex")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE_THROWS_AS(graph.shortest_distance(40, 20), std::out_of_range);
    }

    SECTION("Throws for missing end vertex")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE_THROWS_AS(graph.shortest_distance(10, 40), std::out_of_range);
    }

    SECTION("Returns 0.0 when start equals end")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.shortest_distance(10, 10) == 0.0);
    }

    SECTION("Returns correct distance for two directly connected vertices")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(graph.shortest_distance(10, 20) == 5.0);
    }

    SECTION("Finds a cheaper indirect path")
    {

        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(10, 30, 1.0);
        graph.add_edge(30, 20, 2.0);

        REQUIRE(graph.shortest_distance(10, 20) == 3.0);
    }

    SECTION("Finds a cheapest a path when there are multiple possible paths")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(10, 20, 2.0);
        graph.add_edge(10, 30, 1.0);
        graph.add_edge(20, 50, 1.0);
        graph.add_edge(30, 50, 1.0);
        graph.add_edge(20, 40, 3.0);

        REQUIRE(graph.shortest_distance(10, 50) == 2.0);
    }

    SECTION("Returns correct minimum cost for zero-weight edges")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 2.0);
        graph.add_edge(20, 30, 0.0);

        REQUIRE(graph.shortest_distance(10, 30) == 2.0);
    }

    SECTION("Returns the same distance in undirected graph for reverse direction")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 2.0);
        graph.add_edge(20, 30, 5.0);

        REQUIRE(graph.shortest_distance(10, 30) == 7.0);
        REQUIRE(graph.shortest_distance(30, 10) == 7.0);
        REQUIRE(graph.shortest_distance(10, 30) == graph.shortest_distance(30, 10));
    }

    SECTION("Returns infinity when destination is unreachable")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);

        REQUIRE(std::isinf(graph.shortest_distance(10, 30)));
    }

    SECTION("Handles outdated priority queue entries")
    {

        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20, 10.0);
        graph.add_edge(10, 30, 2.0);
        graph.add_edge(30, 20, 3.0);
        graph.add_edge(20, 40, 1.0);

        REQUIRE(graph.shortest_distance(10, 40) == 6.0);

        graph.add_vertex(50);
        REQUIRE(std::isinf(graph.shortest_distance(10, 50)));
    }

    SECTION("Uses updated edge weights")
    {
        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 10.0);
        graph.add_edge(10, 30, 2.0);
        graph.add_edge(30, 20, 3.0);

        REQUIRE(graph.shortest_distance(10, 20) == 5.0);

        graph.add_edge(10, 20, 1.0);

        REQUIRE(graph.shortest_distance(10, 20) == 1.0);
        REQUIRE(graph.shortest_distance(20, 10) == 1.0);
    }
}

TEST_CASE("Dijkstra shortest_path() operations", "[WeightedGraph]")
{
    SECTION("Throws for missing start vertex")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE_THROWS_AS(graph.shortest_path(30, 20), std::out_of_range);
    }

    SECTION("Throws for missing end vertex")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        REQUIRE_THROWS_AS(graph.shortest_path(10, 30), std::out_of_range);
    }

    SECTION("Returns start when start equal end")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        const DynamicArray<int> expected{10};
        const auto path = graph.shortest_path(10, 10);

        REQUIRE(path.size() == 1);
        REQUIRE(path == expected);
    }

    SECTION("Returns correct path for two vertices with a direct connection")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20, 5.0);

        const DynamicArray<int> expected{10, 20};

        const auto path = graph.shortest_path(10, 20);

        REQUIRE(path.size() == 2);
        REQUIRE(path == expected);
    }

    SECTION("Reconstructs a cheaper indirect path")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(10, 30, 1.0);
        graph.add_edge(30, 20, 2.0);

        const auto path = graph.shortest_path(10, 20);

        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == 10);
        REQUIRE(path[1] == 30);
        REQUIRE(path[2] == 20);
    }

    SECTION("Returns the correct shortest path when there are multiple possible routes")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(10, 20, 2.0);
        graph.add_edge(10, 30, 1.5);
        graph.add_edge(20, 50, 1.0);
        graph.add_edge(30, 50, 1.0);
        graph.add_edge(20, 40, 3.0);

        const auto path = graph.shortest_path(10, 50);

        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == 10);
        REQUIRE(path[1] == 30);
        REQUIRE(path[2] == 50);
    }

    SECTION("Returns an empty array for disconnected vertices")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE(graph.shortest_path(10, 20).empty());
    }

    SECTION("For reverse direction it returns the reversed shortest path")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 5.0);
        graph.add_edge(20, 30, 2.0);

        const auto path = graph.shortest_path(10, 30);

        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == 10);
        REQUIRE(path[1] == 20);
        REQUIRE(path[2] == 30);

        const auto reversed_path = graph.shortest_path(30, 10);

        REQUIRE(reversed_path.size() == 3);
        REQUIRE(reversed_path[0] == 30);
        REQUIRE(reversed_path[1] == 20);
        REQUIRE(reversed_path[2] == 10);
    }

    SECTION("Returns correct path for zero-weight edges")
    {
        WeightedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 2.0);
        graph.add_edge(20, 30, 0.0);

        const auto path = graph.shortest_path(10, 30);

        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == 10);
        REQUIRE(path[1] == 20);
        REQUIRE(path[2] == 30);
    }

    SECTION("Uses updated edge weights")
    {
        WeightedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20, 10.0);
        graph.add_edge(10, 30, 2.0);
        graph.add_edge(30, 20, 3.0);

        const auto path = graph.shortest_path(10, 20);

        REQUIRE(path.size() == 3);
        REQUIRE(path[0] == 10);
        REQUIRE(path[1] == 30);
        REQUIRE(path[2] == 20);

        graph.add_edge(10, 20, 1.0);

        const auto updated_path = graph.shortest_path(10, 20);

        REQUIRE(updated_path.size() == 2);
        REQUIRE(updated_path[0] == 10);
        REQUIRE(updated_path[1] == 20);
    }

    SECTION("Reconstructs a longer shortest path")
    {
        WeightedGraph<int> graph;

        for (int value : {10, 20, 30, 40, 50})
        {
            graph.add_vertex(value);
        }

        graph.add_edge(10, 20, 1.0);
        graph.add_edge(20, 30, 1.0);
        graph.add_edge(30, 40, 1.0);
        graph.add_edge(40, 50, 1.0);
        graph.add_edge(10, 50, 10.0);

        const auto path = graph.shortest_path(10, 50);

        REQUIRE(path.size() == 5);
        REQUIRE(path[0] == 10);
        REQUIRE(path[1] == 20);
        REQUIRE(path[2] == 30);
        REQUIRE(path[3] == 40);
        REQUIRE(path[4] == 50);
    }
}