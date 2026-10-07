#include <catch2/catch_test_macros.hpp>

#include "Graph.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Construction / Empty State
//-------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("Graph is empty after construction", "[Graph]")
{
    Graph<int> graph;

    REQUIRE(graph.empty());
}

TEST_CASE("Graph has zero vertices after construction", "[Graph]")
{
    Graph<int> graph;

    REQUIRE(graph.vertex_count() == 0);
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Element Access
//-------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("Graph contains() returns true when vertex exists", "[Graph]")
{
    Graph<int> graph;
    graph.add_vertex(20);

    REQUIRE(graph.contains(20));
}

TEST_CASE("Graph contains() returns false when vertex does not exist", "[Graph]")
{
    Graph<int> graph;
    graph.add_vertex(20);

    REQUIRE_FALSE(graph.contains(50));
}

TEST_CASE("Graph neighbors() operations", "[Graph]")
{
    SECTION("neighbors() returns the adjacency list of a vertex")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.neighbors(10).getSize() == 1);
    }

    SECTION("add_edge() does not create duplicate edges")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);
        graph.add_edge(10, 20);

        REQUIRE(graph.neighbors(10).getSize() == 1);
        REQUIRE(graph.neighbors(20).getSize() == 1);
    }

    SECTION("neighbors() throws when vertex does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_THROWS_AS(graph.neighbors(30), std::out_of_range);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Edges
//-------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("Graph add / has edge operations", "[Graph]")
{
    SECTION("add_edge() connects two existing vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.has_edge(10, 20));
    }

    SECTION("add_edge() creates an undirected edge")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.has_edge(20, 10));
    }

    SECTION("has_edge() returns false when no edge exists")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
    }

    SECTION("add_edge() does nothing when destination vertex does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        graph.add_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
    }

    SECTION("add_edge() does nothing when source vertex does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
    }

    SECTION("add_edge() ignores self-loops")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        graph.add_edge(10, 10);

        REQUIRE_FALSE(graph.has_edge(10, 10));
    }
}

TEST_CASE("Graph remove_edge() operations", "[Graph]")
{
    SECTION("remove_edge() removes an existing undirected edge")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 10));

        graph.remove_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE_FALSE(graph.has_edge(20, 10));
        REQUIRE(graph.neighbors(10).getSize() == 0);
        REQUIRE(graph.neighbors(20).getSize() == 0);
    }

    SECTION("remove_edge() does nothing when edge does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.remove_edge(10, 20);

        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE(graph.neighbors(10).getSize() == 0);
        REQUIRE(graph.neighbors(20).getSize() == 0);
    }

    SECTION("remove_edge() does nothing when a vertex does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        graph.remove_edge(10, 20);

        REQUIRE(graph.contains(10));
        REQUIRE(graph.vertex_count() == 1);
        REQUIRE(graph.neighbors(10).getSize() == 0);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Modifiers
//-------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("Graph add_vertex() operations", "[Graph]")
{
    SECTION("add_vertex() adds a vertex")
    {
        Graph<int> graph;

        graph.add_vertex(10);

        REQUIRE(graph.vertex_count() == 1);
    }

    SECTION("add_vertex() increases vertex_count for multiple elements")
    {
        Graph<int> graph;

        graph.add_vertex(10);
        REQUIRE(graph.vertex_count() == 1);

        graph.add_vertex(20);
        REQUIRE(graph.vertex_count() == 2);

        graph.add_vertex(30);
        REQUIRE(graph.vertex_count() == 3);
    }

    SECTION("add_vertex() ignores duplicate")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(10);

        REQUIRE(graph.vertex_count() == 1);
    }
}

TEST_CASE("Graph remove_vertex() operations", "[Graph]")
{
    SECTION("remove_vertex() removes an isolated vertex")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.contains(30));

        graph.remove_vertex(20);
        REQUIRE(graph.vertex_count() == 2);
        REQUIRE_FALSE(graph.contains(20));
    }

    SECTION("remove_vertex() removes a connected vertex")
    {
        Graph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(20, 40);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(30, 20));
        REQUIRE(graph.has_edge(40, 20));

        graph.remove_vertex(20);

        REQUIRE(graph.vertex_count() == 3);

        REQUIRE_FALSE(graph.contains(20));
        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE_FALSE(graph.has_edge(30, 20));
        REQUIRE_FALSE(graph.has_edge(40, 20));

        REQUIRE(graph.neighbors(10).getSize() == 0);
        REQUIRE(graph.neighbors(30).getSize() == 0);
        REQUIRE(graph.neighbors(40).getSize() == 0);
    }

    SECTION("remove_vertex() does nothing when vertex does not exist")
    {
        Graph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.vertex_count() == 2);

        graph.remove_vertex(30);

        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.vertex_count() == 2);
    }

    SECTION("remove_vertex() leaves graph empty when removing the only vertex")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        graph.remove_vertex(10);

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
    }

    SECTION("remove_vertex() removes one vertex without affecting unrelated edges")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(30, 40));

        graph.remove_vertex(20);

        REQUIRE(graph.vertex_count() == 3);
        REQUIRE_FALSE(graph.has_edge(10, 20));
        REQUIRE(graph.neighbors(10).getSize() == 0);
        REQUIRE(graph.has_edge(30, 40));
        REQUIRE(graph.has_edge(40, 30));
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Traversals
//-------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("Graph bfs() operations", "[Graph]")
{
    SECTION("bfs() traverses and returns for one vertex")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        const DynamicArray<int> expected{10};

        REQUIRE(graph.bfs(10) == expected);
    }

    SECTION("bfs() traverses a simple connected graph in expected order")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(20, 40);

        const DynamicArray<int> expected{10, 20, 30, 40};

        REQUIRE(graph.bfs(10) == expected);
    }

    SECTION("bfs() handles a cycle without revisiting vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 40);
        graph.add_edge(40, 10);

        const DynamicArray<int> expected{10, 20, 40, 30};

        REQUIRE(graph.bfs(10) == expected);
    }

    SECTION("bfs() only traverses the connected component containing start")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        const DynamicArray<int> expected{10, 20};

        REQUIRE(graph.bfs(10) == expected);
    }

    SECTION("bfs() throws when start doesn't exist")
    {
        Graph<int> graph;
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        REQUIRE_THROWS_AS(graph.bfs(10), std::out_of_range);
    }
}

TEST_CASE("Graph dfs() operations", "[Graph]")
{
    SECTION("dfs() traverses and returns for one vertex")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        const DynamicArray<int> expected{10};

        REQUIRE(graph.dfs(10) == expected);
    }

    SECTION("dfs() traverses a simple connected graph in correct order")
    {
        Graph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(20, 40);

        const DynamicArray<int> expected{10, 20, 40, 30};

        REQUIRE(graph.dfs(10) == expected);
    }

    SECTION("dfs() handles a cycle without revisiting vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(20, 40);
        graph.add_edge(40, 10);

        const DynamicArray<int> expected{10, 40, 20, 30};

        REQUIRE(graph.dfs(10) == expected);
    }

    SECTION("dfs() only traverses the connected component containing start")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        const DynamicArray<int> expected{10, 20};

        REQUIRE(graph.dfs(10) == expected);
    }

    SECTION("dfs() throws when start does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        REQUIRE_THROWS_AS(graph.dfs(10), std::out_of_range);
    }
}