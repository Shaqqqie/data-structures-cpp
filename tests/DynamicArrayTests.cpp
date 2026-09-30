#include <catch2/catch_test_macros.hpp>

#include "DynamicArray.hpp"

TEST_CASE("DynamicArray starts empty")
{
    DynamicArray<int> array;

    REQUIRE(array.size() == 0);
    REQUIRE(array.capacity() == 0);
    REQUIRE(array.empty());
}