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

TEST_CASE("front throws for empty list")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.front(), std::out_of_range);
}