#include <catch2/catch_test_macros.hpp>

#include "WeightedGraph.hpp"

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