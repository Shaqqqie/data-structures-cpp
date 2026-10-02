#include <catch2/catch_test_macros.hpp>

#include "Queue.hpp"

#include <memory>
#include <string>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue is empty after construction", "[Queue]")
{
    Queue<int> queue;

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue push() adds elements", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);
    queue.push(30);

    REQUIRE_FALSE(queue.empty());
    REQUIRE(queue.size() == 3);
}

TEST_CASE("Queue push() maintains FIFO order", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);
    queue.push(30);

    REQUIRE(queue.front() == 10);
    REQUIRE(queue.back() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Front / Back
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue front() returns the first element", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);

    REQUIRE(queue.front() == 10);
}

TEST_CASE("Queue back() returns the last element", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);

    REQUIRE(queue.back() == 20);
}

TEST_CASE("Queue front() and back() return mutable references", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);

    queue.front() = 100;
    queue.back() = 200;

    REQUIRE(queue.front() == 100);
    REQUIRE(queue.back() == 200);
}

TEST_CASE("Const queue supports front and back access", "[Queue]")
{
    Queue<int> queue;
    
    queue.push(10);
    queue.push(20);

    const Queue<int> &const_queue{queue};

    REQUIRE(const_queue.front() == 10);
    REQUIRE(const_queue.back() == 20);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Pop
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue pop() removes the front element", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);
    queue.push(30);

    queue.pop();

    REQUIRE(queue.size() == 2);
    REQUIRE(queue.front() == 20);
    REQUIRE(queue.back() == 30);
}

TEST_CASE("Queue follows FIFO order", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);
    queue.push(30);

    REQUIRE(queue.front() == 10);
    queue.pop();

    REQUIRE(queue.front() == 20);
    queue.pop();

    REQUIRE(queue.front() == 30);
    queue.pop();

    REQUIRE(queue.empty());
}

TEST_CASE("Queue pop() correctly handles the last element", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.pop();

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("Queue can be reused after popping last element", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.pop();

    queue.push(20);

    REQUIRE_FALSE(queue.empty());
    REQUIRE(queue.size() == 1);
    REQUIRE(queue.front() == 20);
    REQUIRE(queue.back() == 20);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Clear
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue clear() removes all elements", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);
    queue.push(30);

    queue.clear();

    REQUIRE(queue.empty());
    REQUIRE(queue.size() == 0);
}

TEST_CASE("Queue can be used after clear()", "[Queue]")
{
    Queue<int> queue;

    queue.push(10);
    queue.push(20);

    queue.clear();

    queue.push(30);

    REQUIRE(queue.size() == 1);
    REQUIRE(queue.front() == 30);
    REQUIRE(queue.back() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Exception Handling
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue pop() throws when empty", "[Queue]")
{
    Queue<int> queue;

    REQUIRE_THROWS_AS(queue.pop(), std::out_of_range);
}

TEST_CASE("Queue front() throws when empty", "[Queue]")
{
    Queue<int> queue;

    REQUIRE_THROWS_AS(queue.front(), std::out_of_range);
}

TEST_CASE("Queue back() throws when empty", "[Queue]")
{
    Queue<int> queue;

    REQUIRE_THROWS_AS(queue.back(), std::out_of_range);
}

TEST_CASE("Const queue front() and back() throw when empty", "[Queue]")
{
    const Queue<int> queue;

    REQUIRE_THROWS_AS(queue.front(), std::out_of_range);
    REQUIRE_THROWS_AS(queue.back(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Generic Type Support
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue works with non-integer types", "[Queue]")
{
    Queue<std::string> queue;

    queue.push("first");
    queue.push("second");
    queue.push("third");

    REQUIRE(queue.front() == "first");
    REQUIRE(queue.back() == "third");

    queue.pop();

    REQUIRE(queue.front() == "second");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Semantics
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue supports move-only types", "[Queue]")
{
    Queue<std::unique_ptr<int>> queue;

    queue.push(std::make_unique<int>(10));
    queue.push(std::make_unique<int>(20));

    REQUIRE(queue.size() == 2);
    REQUIRE(*queue.front() == 10);
    REQUIRE(*queue.back() == 20);

    queue.pop();

    REQUIRE(*queue.front() == 20);
}