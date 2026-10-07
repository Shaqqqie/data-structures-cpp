#include <catch2/catch_test_macros.hpp>

#include "BinaryHeap.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

static_assert(
    std::is_same_v<
        decltype(std::declval<BinaryHeap<int> &>().top()),
        const int &>);

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("BH is empty after construction", "[BH]")
{
    BinaryHeap<int> heap;

    REQUIRE(heap.empty());
    REQUIRE(heap.size() == 0);
}

TEST_CASE("BH constructs min-heap from DynamicArray", "[BH]")
{
    DynamicArray<int> values;

    values.push_back(40);
    values.push_back(10);
    values.push_back(30);
    values.push_back(5);
    values.push_back(20);
    values.push_back(15);

    BinaryHeap<int> heap{values};

    REQUIRE(heap.size() == 6);
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
}

TEST_CASE("BH constructs max-heap from DynamicArray", "[BH]")
{
    DynamicArray<int> values;

    values.push_back(40);
    values.push_back(10);
    values.push_back(30);
    values.push_back(5);
    values.push_back(20);
    values.push_back(15);

    BinaryHeap<int, std::greater<int>> heap{values};

    REQUIRE(heap.size() == 6);
    REQUIRE(heap.top() == 40);

    heap.pop();
    REQUIRE(heap.top() == 30);

    heap.pop();
    REQUIRE(heap.top() == 20);

    heap.pop();
    REQUIRE(heap.top() == 15);

    heap.pop();
    REQUIRE(heap.top() == 10);

    heap.pop();
    REQUIRE(heap.top() == 5);

    heap.pop();
    REQUIRE(heap.empty());
}

TEST_CASE("BH constructs heap when DynamicArray has one element", "[BH]")
{
    DynamicArray<int> values;

    values.push_back(42);

    BinaryHeap<int> heap{values};

    REQUIRE(heap.size() == 1);
    REQUIRE(heap.top() == 42);
}

TEST_CASE("BH constructs empty heap from empty DynamicArray", "[BH]")
{
    DynamicArray<int> values;

    BinaryHeap<int> heap{values};

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
// Pop
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

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Copy
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("BH copy constructor creates an identical heap", "[BH]")
{
    BinaryHeap<int> original;

    original.push(30);
    original.push(10);
    original.push(20);

    BinaryHeap<int> copy{original};

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.top() == 10);

    copy.pop();
    REQUIRE(copy.size() == 2);
    REQUIRE(copy.top() == 20);

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 10);
}

TEST_CASE("BH copy assignment replaces existing heap", "[BH]")
{
    BinaryHeap<int> original;
    original.push(30);
    original.push(10);
    original.push(20);

    BinaryHeap<int> copy;
    copy.push(40);
    copy.push(25);

    copy = original;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.top() == 10);

    copy.pop();

    REQUIRE(copy.size() == 2);
    REQUIRE(copy.top() == 20);

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 10);
}

TEST_CASE("BH handles self assignment", "[BH]")
{
    BinaryHeap<int> original;
    original.push(30);
    original.push(10);
    original.push(20);

    original = original;

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 10);
}

TEST_CASE("BH copy preserves comparator behavior", "[BH]")
{
    BinaryHeap<int, std::greater<int>> original;

    original.push(10);
    original.push(30);
    original.push(20);

    BinaryHeap<int, std::greater<int>> copy{original};

    REQUIRE(copy.top() == 30);

    copy.pop();

    REQUIRE(copy.top() == 20);
    REQUIRE(original.top() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Move
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("BH move constructor transfers ownership", "[BH]")
{
    BinaryHeap<int> original;
    original.push(30);
    original.push(10);
    original.push(20);

    BinaryHeap<int> moved{std::move(original)};

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.top() == 10);

    REQUIRE(original.size() == 0);
    REQUIRE(original.empty());
}

TEST_CASE("BH handles move assignment", "[BH]")
{
    BinaryHeap<int> source;
    source.push(30);
    source.push(10);
    source.push(20);

    BinaryHeap<int> destination;
    destination.push(20);
    destination.push(10);

    destination = std::move(source);

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.top() == 10);

    REQUIRE(source.empty());
    REQUIRE(source.size() == 0);
}
