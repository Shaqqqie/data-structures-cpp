#include <catch2/catch_test_macros.hpp>

#include "BinaryHeap.hpp"

#include <functional>
#include <stdexcept>
#include <string>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BH is empty after construction", "[BH]")
{
    BinaryHeap<int> heap;

    REQUIRE(heap.empty());
    REQUIRE(heap.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BH push() adds one element", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(10);

    REQUIRE(heap.size() == 1);
    REQUIRE_FALSE(heap.empty());
}

TEST_CASE("BH push() correctly maintains min-heap property for multiple elements", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(30);
    heap.push(10);
    heap.push(20);
    heap.push(5);
    heap.push(40);

    REQUIRE(heap.size() == 5);
    REQUIRE(heap.top() == 5);
}

TEST_CASE("BH push() accepts rvalue", "[BH]")
{
    BinaryHeap<std::string> heap;

    heap.push(std::string{"banana"});
    heap.push(std::string{"apple"});
    heap.push(std::string{"cherry"});

    REQUIRE(heap.size() == 3);
    REQUIRE(heap.top() == "apple");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Top
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("BH top() returns minimum element", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(30);
    heap.push(10);
    heap.push(20);

    REQUIRE(heap.top() == 10);
}

TEST_CASE("BH top() throws when heap is empty", "[BH]")
{
    BinaryHeap<int> heap;

    REQUIRE_THROWS_AS(heap.top(), std::out_of_range);
}

TEST_CASE("BH top() works with const heap", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(30);
    heap.push(10);

    const BinaryHeap<int> &const_heap{heap};

    REQUIRE(const_heap.top() == 10);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Top
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("BH pop() throws on empty heap", "[BH]")
{
    BinaryHeap<int> heap;

    REQUIRE_THROWS_AS(heap.pop(), std::out_of_range);
}

TEST_CASE("BH pop() removes the only element", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(10);

    heap.pop();

    REQUIRE(heap.empty());
    REQUIRE(heap.size() == 0);
}

TEST_CASE("BH pop() decreases size", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(10);
    heap.push(20);
    heap.push(30);

    REQUIRE(heap.size() == 3);

    heap.pop();

    REQUIRE(heap.size() == 2);
}

TEST_CASE("BH pop() changes top() to the next minimum", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(30);
    heap.push(10);
    heap.push(20);

    REQUIRE(heap.top() == 10);

    heap.pop();

    REQUIRE(heap.top() == 20);
}

TEST_CASE("BH repeated pop() exposes elements in ascending order", "[BH]")
{
    BinaryHeap<int> heap;

    heap.push(30);
    heap.push(10);
    heap.push(20);
    heap.push(5);
    heap.push(40);
    heap.push(15);

    REQUIRE(heap.top() == 5);

    heap.pop();
    REQUIRE(heap.top() == 10);

    heap.pop();
    REQUIRE(heap.top() == 15);

    heap.pop();
    REQUIRE(heap.top() == 20);

    heap.pop();
    REQUIRE(heap.top() == 30);

    heap.pop();
    REQUIRE(heap.top() == 40);

    heap.pop();
    REQUIRE(heap.empty());
    REQUIRE(heap.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Max Heap
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("BH supports max-heap using std::greater", "[BH]")
{
    BinaryHeap<int, std::greater<int>> heap;

    heap.push(30);
    heap.push(10);
    heap.push(20);
    heap.push(5);
    heap.push(40);
    heap.push(15);

    REQUIRE(heap.top() == 40);

    heap.pop();
    REQUIRE(heap.top() == 30);
}