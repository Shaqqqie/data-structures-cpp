#include <catch2/catch_test_macros.hpp>

#include "LinkedList.hpp"
TEST_CASE("Testing default values for newly created LinkedList")
{
    LinkedList<int> list;

    REQUIRE(list.empty());
    REQUIRE(list.getSize() == 0);
}

TEST_CASE("Testing push_front and front")
{
    LinkedList<int> list;
    list.push_front(10);

    REQUIRE_FALSE(list.empty());
    REQUIRE(list.getSize() == 1);
    REQUIRE(list.front() == 10);

    list.push_front(20);

    REQUIRE(list.getSize() == 2);
    REQUIRE(list.front() == 20);
    
}

TEST_CASE("front() throws for empty list")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.front(), std::out_of_range);
}

TEST_CASE("LinkedList can be copied")
{
    LinkedList<int> original;

    original.push_front(10);
    original.push_front(20);

    LinkedList<int> copy{original};

    REQUIRE(copy.getSize() == 2);
    REQUIRE(copy.front() == 20);
}

TEST_CASE("LinkedList can be copy assigned")
{
    LinkedList<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    LinkedList<int> copy;
    copy.push_back(999);

    copy = original;

    REQUIRE(copy.getSize() == 3);
    REQUIRE(copy.front() == 10);
}

TEST_CASE("LinkedList handles self assignment")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);

    list = list;

    REQUIRE(list.getSize() == 2);
    REQUIRE(list.front() == 10);
}

TEST_CASE("LinkedList maintains front and back")
{
    LinkedList<int> list;
    list.push_back(20);
    list.push_front(10);
    list.push_back(30);

    REQUIRE(list.getSize() == 3);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 30);
}

TEST_CASE("back() throws for empty list")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.back(), std::out_of_range);
}

TEST_CASE("pop_front() works correctly for multi-node list")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_front();

    REQUIRE(list.getSize() == 2);

    REQUIRE(list.front() == 20);
    REQUIRE(list.back() == 30);
}

TEST_CASE("pop_front() works correctly for a one-element list")
{
    LinkedList<int> list;
    list.push_front(10);

    list.pop_front();

    REQUIRE(list.empty());
    REQUIRE(list.getSize() == 0);
}

TEST_CASE("pop_front() throws for an empty list")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.pop_front(), std::out_of_range);
}

TEST_CASE("pop_back() works correctly for multi-node list")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_back();

    REQUIRE(list.getSize() == 2);

    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 20);
}

TEST_CASE("pop_back() works correctly for one-element list")
{
    LinkedList<int> list;
    list.push_back(10);

    list.pop_back();

    REQUIRE(list.empty());
    REQUIRE(list.getSize() == 0);
}

TEST_CASE("pop_back() throws for empty list")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.pop_back(), std::out_of_range);
}