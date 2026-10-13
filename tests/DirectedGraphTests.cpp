#include <catch2/catch_test_macros.hpp>

#include "DirectedGraph.hpp"

#include <utility>

//--------------------------------------------------------------------------------------------------------------------------------------------//
// Capacity / State
//-------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("DirectedGraph empty() operations", "[DirectedGraph]")
{
    SECTION("Returns true when no vertices exist")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.empty());
    }

    SECTION("Becomes false after adding a vertex")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.empty());

        graph.add_vertex(10);

        REQUIRE_FALSE(graph.empty());
    }
}

TEST_CASE("DirectedGraph vertex_count() operations", "[DirectedGraph]")
{
    SECTION("Return 0 when no vertices exist")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.vertex_count() == 0);
    }

    SECTION("Increases when adding unique vertices")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.vertex_count() == 0);

        graph.add_vertex(10);

        REQUIRE(graph.vertex_count() == 1);

        graph.add_vertex(20);

        REQUIRE(graph.vertex_count() == 2);
    }
}

TEST_CASE("DirectedGraph edge_count() operations", "[DirectedGraph]")
{
    SECTION("Returns 0 for an empty graph")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.edge_count() == 0);
    }

    SECTION("Returns 0 when there are vertices but no edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 0);
    }

    SECTION("Returns 1 for one existing edge")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE(graph.edge_count() == 0);

        graph.add_edge(10, 20);

        REQUIRE(graph.edge_count() == 1);
    }

    SECTION("Returns correct amount when there are multiple edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(20, 40);

        REQUIRE(graph.edge_count() == 3);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
// Element Access
//-------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("DirectedGraph contains_vertex() operations")
{
    SECTION("Returns true when vertex exists")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE(graph.contains_vertex(10));
    }

    SECTION("Returns false for missing vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE_FALSE(graph.contains_vertex(20));
    }
}

TEST_CASE("DirectedGraph neighbors() operations", "[DirectedGraph]")
{
    SECTION("Returns outgoing neighbors")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);

        const auto result = graph.neighbors(10);

        REQUIRE(result.size() == 2);
        REQUIRE(result[0] == 20);
        REQUIRE(result[1] == 30);
    }

    SECTION("Returns empty array for vertex without outgoing edges")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_edge(10, 20);

        const auto result = graph.neighbors(20);

        REQUIRE(result.empty());
    }

    SECTION("Throws for missing vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE_THROWS_AS(graph.neighbors(20), std::out_of_range);
    }

    SECTION("Modifying resulting array does not modify internal adjacency list")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);

        auto result = graph.neighbors(10);

        REQUIRE(result[0] == 20);
        REQUIRE(result[1] == 30);

        result[0] = 50;

        REQUIRE_FALSE(result[0] == graph.neighbors(10)[0]);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Modifiers
//-------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("DirectedGraph add_vertex() operations", "[DirectedGraph]")
{
    SECTION("Adds a vertex")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.vertex_count() == 0);

        graph.add_vertex(10);

        REQUIRE(graph.vertex_count() == 1);
    }

    SECTION("Ignores for duplicates")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(10);

        REQUIRE(graph.vertex_count() == 1);
    }
}

TEST_CASE("DirectedGraph add_edge() operations", "[DirectedGraph]")
{
    SECTION("Creates an edge from A to B only")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.contains_edge(10, 20));
        REQUIRE_FALSE(graph.contains_edge(20, 10));
    }

    SECTION("Allows edges in opposite directions")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);
        graph.add_edge(20, 10);

        REQUIRE(graph.contains_edge(10, 20));
        REQUIRE(graph.contains_edge(20, 10));
        REQUIRE(graph.edge_count() == 2);
    }

    SECTION("Throws for duplicate edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.edge_count() == 1);

        REQUIRE_THROWS_AS(graph.add_edge(10, 20), std::invalid_argument);

        REQUIRE(graph.edge_count() == 1);
    }

    SECTION("Handles missing source vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_THROWS_AS(graph.add_edge(30, 10), std::out_of_range);
    }

    SECTION("Handles missing destination vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_THROWS_AS(graph.add_edge(10, 30), std::out_of_range);
    }

    SECTION("Rejects self-loops")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE_THROWS_AS(graph.add_edge(10, 10), std::invalid_argument);
    }
}

TEST_CASE("DirectedGraph remove_edge() operations", "[Directedgraph]")
{
    SECTION("Removes edge")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.contains_edge(10, 20));

        graph.remove_edge(10, 20);

        REQUIRE(graph.edge_count() == 0);
        REQUIRE_FALSE(graph.contains_edge(10, 20));
    }

    SECTION("Removing an edge between two vertices connected both ways only removes one edge")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);
        graph.add_edge(20, 10);

        REQUIRE(graph.edge_count() == 2);
        REQUIRE(graph.contains_edge(10, 20));
        REQUIRE(graph.contains_edge(20, 10));

        graph.remove_edge(10, 20);

        REQUIRE(graph.edge_count() == 1);
        REQUIRE(graph.contains_edge(20, 10));
        REQUIRE_FALSE(graph.contains_edge(10, 20));
    }

    SECTION("Throws if either vertex does not exist")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE_THROWS_AS(graph.remove_edge(10, 30), std::out_of_range);
        REQUIRE_THROWS_AS(graph.remove_edge(30, 10), std::out_of_range);
    }

    SECTION("Throws if edge does not exist")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_THROWS_AS(graph.remove_edge(10, 20), std::out_of_range);
    }

    SECTION("Removing an edge preserves unrelated edges")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);
        graph.add_edge(20, 30);

        graph.remove_edge(10, 20);

        REQUIRE_FALSE(graph.contains_edge(10, 20));
        REQUIRE(graph.contains_edge(10, 30));
        REQUIRE(graph.contains_edge(20, 30));

        REQUIRE(graph.edge_count() == 2);
        REQUIRE(graph.vertex_count() == 3);
    }
}

TEST_CASE("DirectedGraph remove_vertex() operations", "[Directedgraph]")
{
    SECTION("Removes an isolated vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.remove_vertex(10);

        REQUIRE(graph.vertex_count() == 1);
        REQUIRE(graph.contains_vertex(20));
        REQUIRE_FALSE(graph.contains_vertex(10));
    }

    SECTION("Removes a vertex with only incoming edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 20);
        graph.add_edge(40, 20);

        REQUIRE(graph.edge_count() == 3);

        graph.remove_vertex(20);

        REQUIRE(graph.edge_count() == 0);
        REQUIRE_FALSE(graph.contains_vertex(20));
        REQUIRE(graph.contains_vertex(10));
        REQUIRE(graph.contains_vertex(30));
        REQUIRE(graph.contains_vertex(40));
    }

    SECTION("Removes a vertex with both incoming and outgoing edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 10);
        graph.add_edge(20, 30);
        graph.add_edge(30, 20);
        graph.add_edge(20, 40);
        graph.add_edge(40, 20);

        REQUIRE(graph.edge_count() == 6);

        graph.remove_vertex(20);

        REQUIRE(graph.edge_count() == 0);
        REQUIRE_FALSE(graph.contains_vertex(20));
        REQUIRE(graph.contains_vertex(10));
        REQUIRE(graph.contains_vertex(30));
        REQUIRE(graph.contains_vertex(40));
    }

    SECTION("Removes a vertex with only outgoing edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);
        graph.add_edge(20, 30);

        REQUIRE(graph.edge_count() == 3);

        graph.remove_vertex(10);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 1);
        REQUIRE_FALSE(graph.contains_vertex(10));
        REQUIRE_FALSE(graph.contains_edge(10, 20));
        REQUIRE_FALSE(graph.contains_edge(10, 30));
        REQUIRE(graph.contains_edge(20, 30));
    }

    SECTION("Removing a vertex preserves unrelated edges")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 40);
        graph.add_edge(40, 10);

        graph.remove_vertex(20);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE(graph.edge_count() == 2);

        REQUIRE_FALSE(graph.contains_vertex(20));
        REQUIRE_FALSE(graph.contains_edge(10, 20));

        REQUIRE(graph.contains_edge(30, 40));
        REQUIRE(graph.contains_edge(40, 10));
    }

    SECTION("Removing the only vertex leaves graph empty")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE_FALSE(graph.empty());
        REQUIRE(graph.vertex_count() == 1);

        graph.remove_vertex(10);

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
    }

    SECTION("Removing a nonexistent vertex throws")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_THROWS_AS(graph.remove_vertex(30), std::out_of_range);
    }
}

TEST_CASE("DirectedGraph bfs() operations", "[DirectedGraph]")
{
    SECTION("Throws for missing start vertex")
    {
        DirectedGraph<int> graph;

        REQUIRE_THROWS_AS(graph.bfs(10), std::out_of_range);
    }

    SECTION("Returns only start vertex when isolated")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        const auto result = graph.bfs(10);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 10);
    }

    SECTION("Traverses vertices in breadth-first order")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);
        graph.add_edge(20, 40);
        graph.add_edge(30, 50);

        const auto result = graph.bfs(10);

        const DynamicArray<int> expected{
            10, 20, 30, 40, 50};

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }
    }

    SECTION("Visits each vertex once for a graph with a cycle")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        const auto result = graph.bfs(10);

        const DynamicArray<int> expected{
            10,
            20,
            30,
        };

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }
    }

    SECTION("Does not visit a disconnected vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        const auto result = graph.bfs(10);

        const DynamicArray<int> expected{
            10, 20, 30};

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }

        REQUIRE(graph.contains_vertex(40));
    }

    SECTION("Does not traverse edges in reverse")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        const auto result = graph.bfs(20);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 20);
    }

    SECTION("Visits shared destination only once")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);
        graph.add_edge(20, 40);
        graph.add_edge(30, 40);

        const auto result = graph.bfs(10);

        const DynamicArray<int> expected{10, 20, 30, 40};

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }
    }
}

TEST_CASE("DirectedGraph dfs() operations", "[DirectedGraph]")
{
    SECTION("Throws for when start vertex does not exist")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        REQUIRE_THROWS_AS(graph.dfs(20), std::out_of_range);
    }

    SECTION("Returns only start vertex for an isolated vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        const auto result = graph.dfs(10);

        const DynamicArray<int> expected{
            10};

        REQUIRE(result.size() == expected.size());
        REQUIRE(result[0] == expected[0]);
    }

    SECTION("Traverses a graph with branching edges")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(20, 40);

        const auto result = graph.dfs(10);

        const DynamicArray<int> expected{
            10, 20, 40, 30};

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }
    }

    SECTION("Processes last pushed neighbor first")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);
        graph.add_edge(20, 40);

        const auto result = graph.dfs(10);

        const DynamicArray<int> expected{10, 30, 20, 40};

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }
    }

    SECTION("Terminates and visits each vertex once when graph contains a directed cycle")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        const auto result = graph.dfs(10);

        const DynamicArray<int> expected{
            10, 20, 30};

        REQUIRE(result.size() == expected.size());

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }
    }

    SECTION("Does not visit unreachable vertices")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);

        const auto result = graph.dfs(10);

        const DynamicArray<int> expected{
            10, 30, 20};

        for (std::size_t i{}; i < result.size(); ++i)
        {
            REQUIRE(result[i] == expected[i]);
        }

        REQUIRE(graph.contains_vertex(40));
    }

    SECTION("Does not traverse edges in reverse")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        const auto result = graph.dfs(20);

        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 20);
    }
}

TEST_CASE("DirectedGraph topological_sort() operations", "[DirectedGraph]")
{
    SECTION("Empty graph returns empty array")
    {
        DirectedGraph<int> graph;

        REQUIRE(graph.topological_sort().empty());
    }

    SECTION("For a graph with a single vertex, returns that vertex")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);

        const auto result = graph.topological_sort();

        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == 10);
    }

    SECTION("Linear DAG returns dependency order")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 40);

        const auto result = graph.topological_sort();

        const DynamicArray<int> expected{
            10, 20, 30, 40};

        REQUIRE(result.size() == 4);
        REQUIRE(result == expected);
    }

    SECTION("Supports DAG with multiple zero-in-degree vertices")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 30);
        graph.add_edge(20, 30);
        graph.add_edge(30, 40);

        const auto result = graph.topological_sort();

        auto position = [&](int value)
        {
            for (std::size_t i{}; i < result.size(); ++i)
            {
                if (result[i] == value)
                {
                    return i;
                }
            }

            throw std::logic_error("Vertex missing from result.");
        };

        REQUIRE(result.size() == 4);

        REQUIRE(position(10) < position(30));
        REQUIRE(position(20) < position(30));
        REQUIRE(position(30) < position(40));
    }

    SECTION("Includes every vertex for a disconnected DAG")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        const auto result = graph.topological_sort();

        REQUIRE(result.size() == 4);
    }

    SECTION("Throws when a disconnected component contains a cycle")
    {
        DirectedGraph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        // Acyclic component
        graph.add_edge(10, 20);

        // Cyclic component
        graph.add_edge(30, 40);
        graph.add_edge(40, 50);
        graph.add_edge(50, 30);

        REQUIRE_THROWS_AS(
            graph.topological_sort(),
            std::logic_error);
    }

    SECTION("Graph with a directed cycle throws")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        REQUIRE_THROWS_AS(graph.topological_sort(), std::logic_error);
    }

    SECTION("Throws for a partially cyclic graph")
    {
        DirectedGraph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 40);
        graph.add_edge(40, 50);
        graph.add_edge(50, 30);

        REQUIRE_THROWS_AS(graph.topological_sort(), std::logic_error);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Copy Operations
//-------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("DirectedGraph copy operations", "[DirectedGraph]")
{
    SECTION("DirectedGraph copy constructor")
    {
        DirectedGraph<int> original;

        original.add_vertex(10);
        original.add_vertex(20);
        original.add_edge(10, 20);

        DirectedGraph<int> copy{original};

        REQUIRE(copy.vertex_count() == 2);
        REQUIRE(copy.edge_count() == 1);

        copy.remove_edge(10, 20);

        REQUIRE_FALSE(copy.contains_edge(10, 20));
        REQUIRE(original.contains_edge(10, 20));
    }

    SECTION("Copy assignment creates an independent graph")
    {
        DirectedGraph<int> source;
        source.add_vertex(10);
        source.add_vertex(20);
        source.add_edge(10, 20);

        DirectedGraph<int> destination;
        destination.add_vertex(30);

        destination = source;

        REQUIRE(destination.vertex_count() == 2);
        REQUIRE(destination.edge_count() == 1);
        REQUIRE(destination.contains_edge(10, 20));
        REQUIRE_FALSE(destination.contains_vertex(30));

        destination.remove_edge(10, 20);

        REQUIRE(source.contains_edge(10, 20));
    }

    SECTION("Self-assignment does not corrupt anything")
    {
        DirectedGraph<int> source;
        source.add_vertex(10);
        source.add_vertex(20);
        source.add_edge(10, 20);

        REQUIRE(source.vertex_count() == 2);
        REQUIRE(source.contains_vertex(10));
        REQUIRE(source.contains_vertex(20));
        REQUIRE(source.contains_edge(10, 20));

        source = source;

        REQUIRE(source.vertex_count() == 2);
        REQUIRE(source.contains_vertex(10));
        REQUIRE(source.contains_vertex(20));
        REQUIRE(source.contains_edge(10, 20));
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Move Operations
//-------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("DirectedGraph move operations", "[DirectedGraph]")
{
    SECTION("Move construction leaves source graph empty")
    {
        DirectedGraph<int> source;
        source.add_vertex(10);
        source.add_vertex(20);
        source.add_edge(10, 20);

        DirectedGraph<int> destination{std::move(source)};

        REQUIRE(destination.vertex_count() == 2);
        REQUIRE(destination.edge_count() == 1);
        REQUIRE(destination.contains_vertex(10));
        REQUIRE(destination.contains_vertex(20));
        REQUIRE(destination.contains_edge(10, 20));

        REQUIRE(source.empty());
        REQUIRE(source.vertex_count() == 0);
        REQUIRE(source.edge_count() == 0);
    }

    SECTION("Move assigning removes destination contents and leaves source empty")
    {
        DirectedGraph<int> source;
        source.add_vertex(10);
        source.add_vertex(20);
        source.add_edge(10, 20);

        DirectedGraph<int> destination;
        destination.add_vertex(40);
        destination.add_vertex(50);
        destination.add_edge(40, 50);

        destination = std::move(source);

        REQUIRE(destination.vertex_count() == 2);
        REQUIRE(destination.edge_count() == 1);
        REQUIRE(destination.contains_vertex(10));
        REQUIRE(destination.contains_vertex(20));
        REQUIRE(destination.contains_edge(10, 20));
        REQUIRE_FALSE(destination.contains_vertex(40));
        REQUIRE_FALSE(destination.contains_vertex(50));
        REQUIRE_FALSE(destination.contains_edge(40, 50));

        REQUIRE(source.empty());
        REQUIRE(source.vertex_count() == 0);
        REQUIRE(source.edge_count() == 0);
    }
}
