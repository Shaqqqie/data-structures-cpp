#include <catch2/catch_test_macros.hpp>

#include "PriorityQueue.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ is empty after construction", "[PQ]")
{
    PriorityQueue<int> queue;

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Push
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ push() adds one element", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(10);

    REQUIRE(queue.size() == 1);
    REQUIRE_FALSE(queue.empty());
}

TEST_CASE("PQ push() correctly maintains max priority queue ordering for multiple elements", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(30);
    queue.push(10);
    queue.push(20);
    queue.push(5);
    queue.push(40);

    REQUIRE(queue.size() == 5);
    REQUIRE(queue.top() == 40);
}

TEST_CASE("PQ push() accepts rvalue", "[PQ]")
{
    PriorityQueue<std::string> queue;

    queue.push(std::string{"banana"});
    queue.push(std::string{"apple"});
    queue.push(std::string{"cherry"});

    REQUIRE(queue.size() == 3);
    REQUIRE(queue.top() == "cherry");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Top
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ top() returns maximum element", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(30);
    queue.push(10);
    queue.push(20);
    queue.push(40);

    REQUIRE(queue.top() == 40);
}

TEST_CASE("PQ top() throws when queue is empty", "[PQ]")
{
    PriorityQueue<int> queue;

    REQUIRE_THROWS_AS(queue.top(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Pop
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ pop() throws on empty queue", "[PQ]")
{
    PriorityQueue<int> queue;

    REQUIRE_THROWS_AS(queue.pop(), std::out_of_range);
}

TEST_CASE("PQ pop() removes the only element", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(10);

    queue.pop();

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("PQ pop() decreases size", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(30);
    queue.push(10);
    queue.push(20);

    REQUIRE(queue.size() == 3);

    queue.pop();

    REQUIRE(queue.size() == 2);
}

TEST_CASE("PQ pop() changes top() to the next maximum", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(20);
    queue.push(10);
    queue.push(30);

    REQUIRE(queue.top() == 30);

    queue.pop();

    REQUIRE(queue.top() == 20);
}

TEST_CASE("PQ repeated pop() exposes elements in descending order", "[PQ]")
{
    PriorityQueue<int> queue;

    queue.push(30);
    queue.push(10);
    queue.push(20);
    queue.push(5);
    queue.push(40);
    queue.push(15);

    REQUIRE(queue.top() == 40);

    queue.pop();
    REQUIRE(queue.top() == 30);

    queue.pop();
    REQUIRE(queue.top() == 20);

    queue.pop();
    REQUIRE(queue.top() == 15);

    queue.pop();
    REQUIRE(queue.top() == 10);

    queue.pop();
    REQUIRE(queue.top() == 5);

    queue.pop();
    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Min Queue
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ supports min queue using std::less", "[PQ]")
{
    PriorityQueue<int, std::less<int>> queue;

    queue.push(30);
    queue.push(10);
    queue.push(20);
    queue.push(5);
    queue.push(40);
    queue.push(15);

    REQUIRE(queue.top() == 5);

    queue.pop();
    REQUIRE(queue.top() == 10);

    queue.pop();
    REQUIRE(queue.top() == 15);

    queue.pop();
    REQUIRE(queue.top() == 20);

    queue.pop();
    REQUIRE(queue.top() == 30);

    queue.pop();
    REQUIRE(queue.top() == 40);

    queue.pop();
    REQUIRE(queue.empty());
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Copy
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ copy constructor creates an identical queue", "[PQ]")
{
    PriorityQueue<int> original;

    original.push(30);
    original.push(10);
    original.push(20);

    PriorityQueue<int> copy{original};

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.top() == 30);

    copy.pop();
    REQUIRE(copy.size() == 2);
    REQUIRE(copy.top() == 20);

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 30);
}

TEST_CASE("PQ copy assignment replaces existing queue", "[PQ]")
{
    PriorityQueue<int> original;
    original.push(30);
    original.push(10);
    original.push(20);

    PriorityQueue<int> copy;
    copy.push(40);
    copy.push(25);

    copy = original;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.top() == 30);

    copy.pop();

    REQUIRE(copy.size() == 2);
    REQUIRE(copy.top() == 20);

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 30);
}

TEST_CASE("PQ handles self assignment", "[PQ]")
{
    PriorityQueue<int> original;
    original.push(30);
    original.push(10);
    original.push(20);

    original = original;

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 30);
}

TEST_CASE("PQ copy preserves comparator behavior", "[PQ]")
{
    PriorityQueue<int, std::less<int>> original;

    original.push(10);
    original.push(30);
    original.push(20);

    PriorityQueue<int, std::less<int>> copy{original};

    REQUIRE(copy.top() == 10);

    copy.pop();

    REQUIRE(copy.top() == 20);
    REQUIRE(original.top() == 10);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Move
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("PQ move constructor transfers ownership", "[PQ]")
{
    PriorityQueue<int> original;
    original.push(30);
    original.push(10);
    original.push(20);

    PriorityQueue<int> moved{std::move(original)};

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.top() == 30);

    REQUIRE(original.size() == 0);
    REQUIRE(original.empty());
}

TEST_CASE("PQ handles move assignment", "[PQ]")
{
    PriorityQueue<int> source;
    source.push(30);
    source.push(10);
    source.push(20);

    PriorityQueue<int> destination;
    destination.push(20);
    destination.push(10);

    destination = std::move(source);

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.top() == 30);

    REQUIRE(source.empty());
    REQUIRE(source.size() == 0);
}