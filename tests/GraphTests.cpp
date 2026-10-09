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

TEST_CASE("Graph edge_count() operations", "[Graph]")
{
    SECTION("edge_count() returns 0 for an empty graph")
    {
        Graph<int> graph;

        REQUIRE(graph.empty());
        REQUIRE(graph.edge_count() == 0);
    }

    SECTION("edge_count() returns zero when there are vertices but no edges")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE(graph.vertex_count() == 2);
        REQUIRE(graph.edge_count() == 0);
    }

    SECTION("edge_count() equals 1 when there is one existing edge")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        REQUIRE(graph.edge_count() == 0);

        graph.add_edge(10, 20);

        REQUIRE(graph.edge_count() == 1);
    }

    SECTION("edge_count() returns correct amount when there are multiple edges")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 40);

        REQUIRE(graph.edge_count() == 3);
    }

    SECTION("edge_count() stays the same when adding duplicate edge")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        REQUIRE(graph.edge_count() == 2);

        graph.add_edge(30, 40);

        REQUIRE(graph.edge_count() == 2);
    }

    SECTION("edge_count() decreases when removing edge")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        REQUIRE(graph.edge_count() == 2);

        graph.remove_edge(30, 40);

        REQUIRE(graph.edge_count() == 1);
    }

    SECTION("edge_count() decreases when removing a vertex")
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

        REQUIRE(graph.edge_count() == 4);

        graph.remove_vertex(10);

        REQUIRE(graph.edge_count() == 2);
    }
}
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

TEST_CASE("Graph clear() operations", "[Graph]")
{
    SECTION("clear() leaves graph empty")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        REQUIRE(graph.vertex_count() == 4);
        REQUIRE(graph.edge_count() == 2);

        graph.clear();

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
        REQUIRE(graph.edge_count() == 0);
    }

    SECTION("clear() double call does nothing")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(30, 40);

        REQUIRE(graph.vertex_count() == 4);
        REQUIRE(graph.edge_count() == 2);

        graph.clear();

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
        REQUIRE(graph.edge_count() == 0);

        graph.clear();

        REQUIRE(graph.empty());
        REQUIRE(graph.vertex_count() == 0);
        REQUIRE(graph.edge_count() == 0);
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

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Copy Construction
//-------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("Graphs handles copy construction", "[Graph]")
{
    Graph<int> original;

    original.add_vertex(10);
    original.add_vertex(20);
    original.add_vertex(30);

    original.add_edge(10, 20);
    original.add_edge(20, 30);

    Graph<int> copy{original};

    REQUIRE(copy.vertex_count() == 3);
    REQUIRE(copy.edge_count() == 2);

    copy.remove_vertex(20);

    REQUIRE(copy.vertex_count() == 2);
    REQUIRE(copy.edge_count() == 0);

    REQUIRE(original.vertex_count() == 3);
    REQUIRE(original.edge_count() == 2);
}

TEST_CASE("Graphs handles copy assignment", "[Graph]")
{
    Graph<int> original;

    original.add_vertex(10);
    original.add_vertex(20);
    original.add_vertex(30);

    original.add_edge(10, 20);
    original.add_edge(20, 30);

    Graph<int> copy;
    copy = original;

    REQUIRE(copy.vertex_count() == 3);
    REQUIRE(copy.edge_count() == 2);

    copy.remove_vertex(20);

    REQUIRE(copy.vertex_count() == 2);
    REQUIRE(copy.edge_count() == 0);

    REQUIRE(original.vertex_count() == 3);
    REQUIRE(original.edge_count() == 2);
}

TEST_CASE("Graphs handles self-assignment", "[Graph]")
{
    Graph<int> original;

    original.add_vertex(10);
    original.add_vertex(20);
    original.add_vertex(30);

    original.add_edge(10, 20);
    original.add_edge(20, 30);

    original = original;

    REQUIRE(original.vertex_count() == 3);
    REQUIRE(original.edge_count() == 2);
}

TEST_CASE("Graphs handles move construction", "[Graph]")
{
    Graph<int> original;

    original.add_vertex(10);
    original.add_vertex(20);
    original.add_vertex(30);

    original.add_edge(10, 20);
    original.add_edge(20, 30);

    Graph<int> copy{std::move(original)};

    REQUIRE(copy.vertex_count() == 3);
    REQUIRE(copy.edge_count() == 2);

    REQUIRE(original.vertex_count() == 0);
    REQUIRE(original.edge_count() == 0);
}

//--------------------------------------------------------------------------------------------------------------------------------------------//
//  Algorithms
//-------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("Graph has_cycle() operations", "[Graph]")
{
    SECTION("returns false for an empty graph")
    {
        Graph<int> graph;

        REQUIRE_FALSE(graph.has_cycle());
    }

    SECTION("Returns false for a graph with as single vertex")
    {
        Graph<int> graph;

        graph.add_vertex(10);

        REQUIRE_FALSE(graph.has_cycle());
    }

    SECTION("Returns false for two connected vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        REQUIRE_FALSE(graph.has_cycle());
    }

    SECTION("Detects a triangle")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        REQUIRE(graph.has_cycle());
    }

    SECTION("Detects a cycle in a disconnected graph")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);
        graph.add_vertex(60);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        graph.add_edge(40, 50);
        graph.add_edge(50, 60);
        graph.add_edge(60, 40);

        REQUIRE(graph.has_cycle());
    }

    SECTION("Returns false for disconnected graph with no cycle")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);
        graph.add_vertex(60);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        graph.add_edge(40, 50);
        graph.add_edge(50, 60);

        REQUIRE_FALSE(graph.has_cycle());
    }
}

TEST_CASE("Graph connected_components() operations", "[Graph]")
{
    SECTION("Returns 0 for an empty graph")
    {
        Graph<int> graph;

        REQUIRE(graph.connected_components() == 0);
    }

    SECTION("Returns 1 for a single vertex")
    {
        Graph<int> graph;
        graph.add_vertex(10);

        REQUIRE(graph.connected_components() == 1);
    }

    SECTION("Returns 1 for three connected vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        REQUIRE(graph.connected_components() == 1);
    }

    SECTION("Returns 3 for three isolated vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        REQUIRE(graph.connected_components() == 3);
    }

    SECTION("Returns 2 for two disconnected components")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(40, 50);

        REQUIRE(graph.connected_components() == 2);
    }

    SECTION("Returns 1 for graph with a cycle")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        REQUIRE(graph.connected_components() == 1);
    }

    SECTION("Returns correct total for graph with mixed components and isolated vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);
        graph.add_vertex(60);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        graph.add_edge(40, 50);

        REQUIRE(graph.connected_components() == 3);
    }

    SECTION("Removing an edge can split a connected component")
    {
        Graph<int> graph;

        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        REQUIRE(graph.connected_components() == 1);

        graph.remove_edge(20, 30);

        REQUIRE(graph.connected_components() == 2);
    }
}

TEST_CASE("Graph shortest_path() operations", "[Graph]")
{
    SECTION("Throws when start vertex does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        REQUIRE_THROWS_AS(graph.shortest_path(15, 30), std::out_of_range);
    }

    SECTION("Throws when end vertex does not exist")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        REQUIRE_THROWS_AS(graph.shortest_path(10, 50), std::out_of_range);
    }

    SECTION("Returns one vertex when start equals end")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        const DynamicArray<int> expected{10};

        const auto result = graph.shortest_path(10, 10);

        REQUIRE(result.size() == 1);
        REQUIRE(result == expected);
    }

    SECTION("Returns the correct path for two directly connected vertices")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        graph.add_edge(10, 20);

        const DynamicArray<int> expected{10, 20};

        REQUIRE(graph.shortest_path(10, 20) == expected);
    }

    SECTION("Returns the correct path for three vertices in a chain")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);

        const DynamicArray<int> expected{10, 20, 30};

        const auto result = graph.shortest_path(10, 30);

        REQUIRE(result.size() == 3);
        REQUIRE(result == expected);
    }

    SECTION("Returns a shortest path when there are multiple possible paths")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);
        graph.add_vertex(40);
        graph.add_vertex(50);

        graph.add_edge(10, 20);
        graph.add_edge(10, 30);
        graph.add_edge(20, 40);
        graph.add_edge(20, 50);
        graph.add_edge(30, 50);

        const DynamicArray<int> result{graph.shortest_path(10, 50)};

        REQUIRE(result.size() == 3);
        REQUIRE(result[0] == 10);
        REQUIRE(result[2] == 50);

        REQUIRE((result[1] == 20 || result[1] == 30));
    }

    SECTION("Returns empty array when vertices are disconnected")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);

        const DynamicArray<int> expected{};

        REQUIRE(graph.shortest_path(10, 20) == expected);
    }

    SECTION("BFS does not revisit vertices indefinitely for graph with a cycle")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        const DynamicArray<int> expected{10, 30};

        const auto result = graph.shortest_path(10, 30);

        REQUIRE(result.size() == 2);
        REQUIRE(result == expected);
    }

    SECTION("Does not modify vertices or edges")
    {
        Graph<int> graph;
        graph.add_vertex(10);
        graph.add_vertex(20);
        graph.add_vertex(30);

        graph.add_edge(10, 20);
        graph.add_edge(20, 30);
        graph.add_edge(30, 10);

        const auto result = graph.shortest_path(10, 30);

        REQUIRE(graph.contains(10));
        REQUIRE(graph.contains(20));
        REQUIRE(graph.contains(30));
        REQUIRE(graph.has_edge(10, 20));
        REQUIRE(graph.has_edge(20, 30));
        REQUIRE(graph.has_edge(30, 10));
    }
}