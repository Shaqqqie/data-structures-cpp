#include <catch2/catch_test_macros.hpp>

#include "DynamicArray.hpp"
#include "BinarySearchTree.hpp"

#include <stdexcept>
#include <utility>

namespace
{
    struct ThrowOnCopy
    {
        int value{};

        static inline int copies_until_throw{-1};

        ThrowOnCopy() = default;

        explicit ThrowOnCopy(int value)
            : value{value}
        {
        }

        ThrowOnCopy(const ThrowOnCopy &other)
            : value{other.value}
        {
            if (copies_until_throw == 0)
            {
                throw std::runtime_error("Copy failed.");
            }

            if (copies_until_throw > 0)
            {
                --copies_until_throw;
            }
        }

        bool operator<(const ThrowOnCopy &other) const
        {
            return value < other.value;
        }

        bool operator>(const ThrowOnCopy &other) const
        {
            return value > other.value;
        }

        bool operator==(const ThrowOnCopy &other) const
        {
            return value == other.value;
        }
    };
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree is empty after construction", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Capacity / State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree height() returns 0 for empty tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    REQUIRE(tree.height() == 0);
}

TEST_CASE("BinarySearchTree height() returns 1 for one node", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);

    REQUIRE(tree.height() == 1);
}

TEST_CASE("BinarySearchTree height() returns correct height for left-skewed tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(9);
    tree.insert(8);
    tree.insert(5);
    tree.insert(2);

    REQUIRE(tree.height() == 5);
}

TEST_CASE("BinarySearchTree height() returns correct height for right-skewed tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);

    REQUIRE(tree.height() == 4);
}

TEST_CASE("BinarySearchTree height() returns correct height when longest path is somewhere on the right", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(8);
    tree.insert(20);
    tree.insert(25);
    tree.insert(15);
    tree.insert(45);

    REQUIRE(tree.height() == 4);
}

TEST_CASE("BinarySearchTree height() returns correct height after erase()", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(8);
    tree.insert(20);
    tree.insert(25);
    tree.insert(15);
    tree.insert(45);

    tree.erase(25);

    REQUIRE(tree.height() == 3);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Element Access
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree contains() returns false for an empty tree", "[BinarySearchTree]")
{
    const BinarySearchTree<int> tree;

    REQUIRE_FALSE(tree.contains(10));
}

TEST_CASE("BinarySearchTree contains() returns true for existing values", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);

    REQUIRE(tree.contains(10));
    REQUIRE(tree.contains(2));
    REQUIRE(tree.contains(7));
    REQUIRE(tree.contains(20));
}

TEST_CASE("BinarySearchTree contains() returns false for missing values", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    REQUIRE_FALSE(tree.contains(1));
    REQUIRE_FALSE(tree.contains(7));
    REQUIRE_FALSE(tree.contains(30));
}

TEST_CASE("BinarySearchTree min() returns the smallest value", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);
    tree.insert(1);

    REQUIRE(tree.min() == 1);
}

TEST_CASE("BinarySearchTree max() returns the largest value", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(15);
    tree.insert(30);
    tree.insert(40);

    REQUIRE(tree.max() == 40);
}

TEST_CASE("BinarySearchTree min() and max() work with a single element", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(42);

    REQUIRE(tree.min() == 42);
    REQUIRE(tree.max() == 42);
}

TEST_CASE("BinarySearchTree min() throws when tree is empty", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    REQUIRE_THROWS_AS(tree.min(), std::out_of_range);
}

TEST_CASE("BinarySearchTree max() throws when tree is empty", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    REQUIRE_THROWS_AS(tree.max(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Modifiers
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree insert() adds a root node", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);

    REQUIRE_FALSE(tree.empty());
    REQUIRE(tree.size() == 1);
    REQUIRE(tree.contains(10));
}

TEST_CASE("BinarySearchTree insert() adds values according to BST ordering", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);
    tree.insert(15);
    tree.insert(30);

    REQUIRE(tree.size() == 7);

    REQUIRE(tree.contains(10));
    REQUIRE(tree.contains(5));
    REQUIRE(tree.contains(20));
    REQUIRE(tree.contains(2));
    REQUIRE(tree.contains(7));
    REQUIRE(tree.contains(15));
    REQUIRE(tree.contains(30));
}

TEST_CASE("BinarySearchTree insert() ignores duplicate values", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(10);
    tree.insert(5);

    REQUIRE(tree.size() == 2);

    REQUIRE(tree.inorder() == DynamicArray<int>{5, 10});
}

TEST_CASE("BinarySearchTree clear() removes all values", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);

    tree.clear();

    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
    REQUIRE_FALSE(tree.contains(10));
    REQUIRE(tree.inorder().empty());
}

TEST_CASE("BinarySearchTree clear() works on an empty tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.clear();

    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
}

TEST_CASE("BinarySearchTree can be reused after clear()", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    tree.clear();

    tree.insert(100);
    tree.insert(50);
    tree.insert(150);

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.min() == 50);
    REQUIRE(tree.max() == 150);
    REQUIRE(tree.inorder() == DynamicArray<int>{50, 100, 150});
}

TEST_CASE("BinarySearchTree erase() doesn't change tree when value not in tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    REQUIRE_FALSE(tree.erase(99));
    REQUIRE(tree.size() == 3);
}

TEST_CASE("BinarySearchTree erase() removes a leaf", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    tree.erase(5);

    const DynamicArray<int> expected{10, 20};

    REQUIRE(tree.size() == 2);
    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree erase() removes the only node", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);

    tree.erase(10);

    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
}

TEST_CASE("BinarySearchTree erase() removes node with only right child", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(7);

    tree.erase(5);

    const DynamicArray<int> expected{7, 10};

    REQUIRE(tree.size() == 2);
    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree erase() removes node with only left child", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(15);

    tree.erase(20);

    const DynamicArray<int> expected{10, 15};

    REQUIRE(tree.size() == 2);
    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree erase() removes root with one child", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(20);

    tree.erase(10);

    const DynamicArray<int> expected{20};

    REQUIRE(tree.size() == 1);
    REQUIRE_FALSE(tree.contains(10));
    REQUIRE(tree.contains(20));
    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree erase() removes node with two children when successor is direct right child", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(30);

    tree.erase(10);

    const DynamicArray<int> expected{5, 20, 30};

    REQUIRE_FALSE(tree.contains(10));
    REQUIRE(tree.size() == 3);
    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree erase() removes node with two children when successor is deeper", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(15);
    tree.insert(30);
    tree.insert(12);

    tree.erase(10);

    const DynamicArray<int> expected{5, 12, 15, 20, 30};

    REQUIRE(tree.size() == 5);
    REQUIRE_FALSE(tree.contains(10));
    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree erase() reconnects successor's right child", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(15);
    tree.insert(12);
    tree.insert(13);

    tree.erase(10);

    const DynamicArray<int> expected{5, 12, 13, 15, 20};

    REQUIRE(tree.size() == 5);
    REQUIRE_FALSE(tree.contains(10));
    REQUIRE(tree.inorder() == expected);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// DFS Traversals
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree inorder() returns values in sorted order", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);
    tree.insert(15);
    tree.insert(30);

    const DynamicArray<int> expected{2, 5, 7, 10, 15, 20, 30};

    REQUIRE(tree.inorder() == expected);
}

TEST_CASE("BinarySearchTree preorder() traverses node left right", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);
    tree.insert(15);
    tree.insert(30);

    const DynamicArray<int> expected{10, 5, 2, 7, 20, 15, 30};

    REQUIRE(tree.preorder() == expected);
}

TEST_CASE("BinarySearchTree postorder() traverses left right node", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(2);
    tree.insert(7);
    tree.insert(15);
    tree.insert(30);

    const DynamicArray<int> expected{2, 7, 5, 15, 30, 20, 10};

    REQUIRE(tree.postorder() == expected);
}

TEST_CASE("BinarySearchTree DFS traversals return empty arrays for an empty tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    REQUIRE(tree.inorder().empty());
    REQUIRE(tree.preorder().empty());
    REQUIRE(tree.postorder().empty());
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// BFS Traversals
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree level_order returns empty array when tree is empty", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    const DynamicArray<int> result{tree.level_order()};

    REQUIRE(result.empty());
    REQUIRE(result.size() == 0);
}

TEST_CASE("BinarySearchTree level_order() returns one element when only one node in tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);

    const DynamicArray<int> result{tree.level_order()};

    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == 10);
}

TEST_CASE("BinarySearchTree level_order() returns correct order of elements for a balanced tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);
    tree.insert(15);
    tree.insert(7);
    tree.insert(2);

    const DynamicArray<int> result{tree.level_order()};

    REQUIRE(result == DynamicArray<int> {10, 5, 20, 2, 7, 15});
}

TEST_CASE("BinarySearchTree level_order() returns correct order for an unbalanced tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(7);
    tree.insert(5);
    tree.insert(2);
    tree.insert(12);

    const DynamicArray<int> result{tree.level_order()};

    REQUIRE(result == DynamicArray<int> {10, 7, 12, 5, 2});
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy Construction
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree copy constructor creates an identical tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> original;

    original.insert(10);
    original.insert(5);
    original.insert(20);
    original.insert(2);
    original.insert(7);

    BinarySearchTree<int> copy{original};

    REQUIRE(copy.size() == original.size());
    REQUIRE(copy.inorder() == original.inorder());
    REQUIRE(copy.preorder() == original.preorder());
}

TEST_CASE("BinarySearchTree copy constructor creates an independent deep copy", "[BinarySearchTree]")
{
    BinarySearchTree<int> original;

    original.insert(10);
    original.insert(5);
    original.insert(20);

    BinarySearchTree<int> copy{original};

    copy.insert(15);

    REQUIRE(copy.contains(15));
    REQUIRE_FALSE(original.contains(15));

    REQUIRE(copy.size() == 4);
    REQUIRE(original.size() == 3);
}

TEST_CASE("BinarySearchTree copy constructor copies an empty tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> original;

    BinarySearchTree<int> copy{original};

    REQUIRE(copy.empty());
    REQUIRE(copy.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy Assignment
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree copy assignment replaces existing tree", "[BinarySearchTree]")
{
    BinarySearchTree<int> source;

    source.insert(10);
    source.insert(5);
    source.insert(20);

    BinarySearchTree<int> destination;

    destination.insert(100);
    destination.insert(50);
    destination.insert(150);

    destination = source;

    REQUIRE(destination.size() == source.size());
    REQUIRE(destination.inorder() == source.inorder());

    REQUIRE_FALSE(destination.contains(100));
    REQUIRE_FALSE(destination.contains(50));
    REQUIRE_FALSE(destination.contains(150));
}

TEST_CASE("BinarySearchTree copy assignment creates an independent deep copy", "[BinarySearchTree]")
{
    BinarySearchTree<int> source;

    source.insert(10);
    source.insert(5);
    source.insert(20);

    BinarySearchTree<int> destination;

    destination = source;

    destination.insert(15);

    REQUIRE(destination.contains(15));
    REQUIRE_FALSE(source.contains(15));

    REQUIRE(destination.size() == 4);
    REQUIRE(source.size() == 3);
}

TEST_CASE("BinarySearchTree handles copy self-assignment", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    tree = tree;

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.inorder() == DynamicArray<int>{5, 10, 20});
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Construction
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree move constructor transfers ownership", "[BinarySearchTree]")
{
    BinarySearchTree<int> source;

    source.insert(10);
    source.insert(5);
    source.insert(20);

    BinarySearchTree<int> destination{std::move(source)};

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.inorder() == DynamicArray<int>{5, 10, 20});

    REQUIRE(source.empty());
    REQUIRE(source.size() == 0);
}

TEST_CASE("BinarySearchTree moved-from object can be reused after move construction", "[BinarySearchTree]")
{
    BinarySearchTree<int> source;

    source.insert(10);
    source.insert(5);
    source.insert(20);

    BinarySearchTree<int> destination{std::move(source)};

    source.insert(100);

    REQUIRE(source.size() == 1);
    REQUIRE(source.contains(100));

    REQUIRE(destination.size() == 3);
    REQUIRE_FALSE(destination.contains(100));
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Assignment
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree move assignment transfers ownership", "[BinarySearchTree]")
{
    BinarySearchTree<int> source;

    source.insert(10);
    source.insert(5);
    source.insert(20);

    BinarySearchTree<int> destination;

    destination.insert(100);
    destination.insert(50);

    destination = std::move(source);

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.inorder() == DynamicArray<int>{5, 10, 20});

    REQUIRE_FALSE(destination.contains(100));
    REQUIRE_FALSE(destination.contains(50));

    REQUIRE(source.empty());
    REQUIRE(source.size() == 0);
}

TEST_CASE("BinarySearchTree moved-from object can be reused after move assignment", "[BinarySearchTree]")
{
    BinarySearchTree<int> source;

    source.insert(10);
    source.insert(5);
    source.insert(20);

    BinarySearchTree<int> destination;

    destination = std::move(source);

    source.insert(100);

    REQUIRE(source.size() == 1);
    REQUIRE(source.contains(100));

    REQUIRE(destination.size() == 3);
    REQUIRE_FALSE(destination.contains(100));
}

TEST_CASE("BinarySearchTree handles move self-assignment", "[BinarySearchTree]")
{
    BinarySearchTree<int> tree;

    tree.insert(10);
    tree.insert(5);
    tree.insert(20);

    tree = std::move(tree);

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.inorder() == DynamicArray<int>{5, 10, 20});
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Exception Safety
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BinarySearchTree copy constructor cleans up if element copying throws", "[BinarySearchTree]")
{
    BinarySearchTree<ThrowOnCopy> tree;

    tree.insert(ThrowOnCopy{10});
    tree.insert(ThrowOnCopy{5});
    tree.insert(ThrowOnCopy{20});
    tree.insert(ThrowOnCopy{2});
    tree.insert(ThrowOnCopy{7});

    ThrowOnCopy::copies_until_throw = 3;

    REQUIRE_THROWS_AS(
        BinarySearchTree<ThrowOnCopy>{tree},
        std::runtime_error);

    ThrowOnCopy::copies_until_throw = -1;

    REQUIRE(tree.size() == 5);
}





