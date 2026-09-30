#include <catch2/catch_test_macros.hpp>

#include "DynamicArray.hpp"

TEST_CASE("DynamicArray starts empty")
{
    DynamicArray<int> array;

    REQUIRE(array.size() == 0);
    REQUIRE(array.capacity() == 0);
    REQUIRE(array.empty());
}

TEST_CASE("DynamicArray can push back elements")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    REQUIRE(array.size() == 3);
    REQUIRE(array.capacity() == 4);

    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
    REQUIRE(array[2] == 30);
}

TEST_CASE("DynamicArray operator[] allows element modification")
{
    DynamicArray<int> array;

    array.push_back(10);
    array[0] = 42;

    REQUIRE(array[0] == 42);
}
