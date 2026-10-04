#include <catch2/catch_test_macros.hpp>

#include "Deque.hpp"

#include <memory>
#include <string>
#include <type_traits>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque is empty after construction", "[Deque]")
{
    Deque<int> deque;

    REQUIRE(deque.empty());
    REQUIRE(deque.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque push_front() adds elements to the front", "[Deque]")
{
    Deque<int> deque;

    deque.push_front(10);
    deque.push_front(20);
    deque.push_front(30);

    REQUIRE(deque.size() == 3);
    REQUIRE(deque.front() == 30);
    REQUIRE(deque.back() == 10);
}

TEST_CASE("Deque push_back() adds elements to the back", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(10);
    deque.push_back(20);
    deque.push_back(30);

    REQUIRE(deque.size() == 3);
    REQUIRE(deque.front() == 10);
    REQUIRE(deque.back() == 30);
}

TEST_CASE("Deque push_front() and push_back() maintain correct order", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(20);
    deque.push_front(10);
    deque.push_back(30);
    deque.push_front(5);

    REQUIRE(deque.size() == 4);
    REQUIRE(deque.front() == 5);
    REQUIRE(deque.back() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Pop
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque pop_front() removes elements from the front", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(10);
    deque.push_back(20);
    deque.push_back(30);

    deque.pop_front();

    REQUIRE(deque.size() == 2);
    REQUIRE(deque.front() == 20);
    REQUIRE(deque.back() == 30);

    deque.pop_front();

    REQUIRE(deque.size() == 1);
    REQUIRE(deque.front() == 30);
    REQUIRE(deque.back() == 30);
}

TEST_CASE("Deque pop_back() removes elements from the back", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(10);
    deque.push_back(20);
    deque.push_back(30);

    deque.pop_back();

    REQUIRE(deque.size() == 2);
    REQUIRE(deque.front() == 10);
    REQUIRE(deque.back() == 20);

    deque.pop_back();

    REQUIRE(deque.size() == 1);
    REQUIRE(deque.front() == 10);
    REQUIRE(deque.back() == 10);
}

TEST_CASE("Deque becomes empty after removing its last element", "[Deque]")
{
    SECTION("pop_front()")
    {
        Deque<int> deque;
        deque.push_back(10);

        deque.pop_front();

        REQUIRE(deque.empty());
        REQUIRE(deque.size() == 0);
    }

    SECTION("pop_back()")
    {
        Deque<int> deque;
        deque.push_front(10);

        deque.pop_back();

        REQUIRE(deque.empty());
        REQUIRE(deque.size() == 0);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Front / Back
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque front() and back() return mutable references", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(10);
    deque.push_back(20);

    deque.front() = 100;
    deque.back() = 200;

    static_assert(std::is_same_v<decltype(deque.front()), int &>);
    static_assert(std::is_same_v<decltype(deque.back()), int &>);

    REQUIRE(deque.front() == 100);
    REQUIRE(deque.back() == 200);
}

TEST_CASE("Const deque supports front and back access", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(10);
    deque.push_back(20);

    const Deque<int> &const_deque{deque};

    static_assert(std::is_same_v<decltype(const_deque.front()), const int &>);
    static_assert(std::is_same_v<decltype(const_deque.back()), const int &>);

    REQUIRE(const_deque.front() == 10);
    REQUIRE(const_deque.back() == 20);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Mixed operations
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque supports mixed operations on both ends", "[Deque]")
{
    Deque<int> deque;

    deque.push_back(20);
    deque.push_front(10);
    deque.push_back(30);
    deque.push_front(5);

    deque.pop_front();

    deque.pop_back();

    deque.push_front(1);

    deque.push_back(40);

    REQUIRE(deque.size() == 4);
    REQUIRE(deque.front() == 1);
    REQUIRE(deque.back() == 40);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Exception Handling
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque operations throw when deque is empty", "[Deque]")
{
    SECTION("front()")
    {
        Deque<int> deque;

        REQUIRE_THROWS_AS(deque.front(), std::out_of_range);
    }

    SECTION("back()")
    {
        Deque<int> deque;

        REQUIRE_THROWS_AS(deque.back(), std::out_of_range);
    }

    SECTION("pop_front()")
    {
        Deque<int> deque;

        REQUIRE_THROWS_AS(deque.pop_front(), std::out_of_range);
    }

    SECTION("pop_back()")
    {
        Deque<int> deque;

        REQUIRE_THROWS_AS(deque.pop_back(), std::out_of_range);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Generic Type Support
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque works with non-integer types", "[Deque]")
{
    Deque<std::string> deque;

    deque.push_back("second");
    deque.push_front("first");
    deque.push_back("third");

    REQUIRE(deque.size() == 3);
    REQUIRE(deque.front() == "first");
    REQUIRE(deque.back() == "third");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy Semantics
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque copy construction creates an independent copy", "[Deque]")
{
    Deque<int> original;

    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    Deque<int> copy{original};

    copy.front() = 100;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.front() == 100);
    REQUIRE(copy.back() == 30);

    REQUIRE(original.size() == 3);
    REQUIRE(original.front() == 10);
    REQUIRE(original.back() == 30);
}

TEST_CASE("Deque copy assignment creates an independent copy", "[Deque]")
{
    Deque<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    Deque<int> copy;
    copy.push_back(999);

    copy = original;

    copy.front() = 100;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.front() == 100);
    REQUIRE(copy.back() == 30);

    REQUIRE(original.size() == 3);
    REQUIRE(original.front() == 10);
    REQUIRE(original.back() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Semantics
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Deque supports move-only types", "[Deque]")
{
    Deque<std::unique_ptr<int>> deque;

    auto first = std::make_unique<int>(10);
    auto second = std::make_unique<int>(20);

    deque.push_front(std::move(first));
    deque.push_back(std::move(second));

    REQUIRE(first == nullptr);
    REQUIRE(second == nullptr);

    REQUIRE(deque.size() == 2);
    REQUIRE(*deque.front() == 10);
    REQUIRE(*deque.back() == 20);
}

TEST_CASE("Deque move construction transfers ownership", "[Deque]")
{
    Deque<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    Deque<int> moved{std::move(original)};

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.front() == 10);
    REQUIRE(moved.back() == 30);

    REQUIRE(original.empty());
    REQUIRE(original.size() == 0);
}

TEST_CASE("Deque move assignment transfers ownership", "[Deque]")
{
    Deque<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    Deque<int> moved;
    moved.push_back(999);

    moved = std::move(original);

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.front() == 10);
    REQUIRE(moved.back() == 30);

    REQUIRE(original.empty());
    REQUIRE(original.size() == 0);
}
