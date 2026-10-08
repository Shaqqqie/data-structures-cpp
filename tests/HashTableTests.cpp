#include <catch2/catch_test_macros.hpp>

#include "HashTable.hpp"

#include <stdexcept>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Construction / State
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("HashTable construction operations and state checking", "[HashTable]")
{
    SECTION("Default construction creates 8 buckets and 0 entries")
    {
        HashTable<int, int> table;

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE(table.bucket_count() == 8);
    }

    SECTION("Construction with inputted bucket_count works")
    {
        HashTable<int, int> table(16);

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE(table.bucket_count() == 16);
    }

    SECTION("Construction throws when bucket_count equals 0")
    {
        REQUIRE_THROWS_AS((HashTable<int, int>{0}), std::invalid_argument);
    }
}