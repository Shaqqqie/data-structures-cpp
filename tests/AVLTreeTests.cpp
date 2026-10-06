#include <catch2/catch_test_macros.hpp>

#include "AVLTree.hpp"

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("AVL is empty after construction", "[AVL]")
{
    AVLTree<int> tree;

    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Modifiers
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("AVL insert() adds one element", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(10);

    REQUIRE_FALSE(tree.empty());
    REQUIRE(tree.size() == 1);
}

TEST_CASE("AVL insert() adds multiple elements", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.contains(10));
    REQUIRE(tree.contains(5));
    REQUIRE(tree.contains(20));
}

TEST_CASE("AVL ignores duplicate values", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(10);
    tree.insert(10);

    REQUIRE(tree.size() == 1);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Element Access
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("AVL contains() returns true for inserted values", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.contains(20));
    REQUIRE(tree.contains(10));
    REQUIRE(tree.contains(30));
    REQUIRE_FALSE(tree.contains(999));
}

TEST_CASE("AVL contains() returns false for empty tree", "[AVL]")
{
    AVLTree<int> tree;

    REQUIRE_FALSE(tree.contains(999));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Rotations
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("AVL rotates correctly for LL", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(30);
    tree.insert(20);
    tree.insert(10);

    const DynamicArray<int> result{tree.preorder()};

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 20);
    REQUIRE(result[1] == 10);
    REQUIRE(result[2] == 30);
}

TEST_CASE("AVL rotates correctly for RR", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    const DynamicArray<int> result{tree.preorder()};

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 20);
    REQUIRE(result[1] == 10);
    REQUIRE(result[2] == 30);
}

TEST_CASE("AVL rotates correctly for LR", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(30);
    tree.insert(10);
    tree.insert(20);

    const DynamicArray<int> result{tree.preorder()};

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 20);
    REQUIRE(result[1] == 10);
    REQUIRE(result[2] == 30);
}

TEST_CASE("AVL rotates correctly for RL", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(10);
    tree.insert(30);
    tree.insert(20);

    const DynamicArray<int> result{tree.preorder()};

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == 20);
    REQUIRE(result[1] == 10);
    REQUIRE(result[2] == 30);
}

TEST_CASE("AVL rotations preserve subtrees", "[AVL]")
{
    AVLTree<int> tree;

    tree.insert(30);
    tree.insert(20);
    tree.insert(25);
    tree.insert(10);

    const DynamicArray<int> result{tree.preorder()};

    REQUIRE(result.size() == 4);
    REQUIRE(result[0] == 25);
    REQUIRE(result[1] == 20);
    REQUIRE(result[2] == 10);
    REQUIRE(result[3] == 30);

    REQUIRE(tree.contains(30));
    REQUIRE(tree.contains(20));
    REQUIRE(tree.contains(10));
    REQUIRE(tree.contains(25));
}
