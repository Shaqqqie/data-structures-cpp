#include <catch2/catch_test_macros.hpp>

#include "HashTable.hpp"

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Element Access
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("HashTable contains() operations", "[HashTable]")
{
    SECTION("contains() returns false for empty HashTable")
    {
        HashTable<int, int> table;

        REQUIRE_FALSE(table.contains(10));
    }

    SECTION("contains() returns true for an existing key")
    {
        HashTable<int, int> table;

        table.insert(42, 25);

        REQUIRE(table.contains(42));
    }

    SECTION("contains() returns false for missing key in non-empty HashTable")
    {
        HashTable<int, int> table;

        table.insert(42, 25);
        table.insert(27, 15);

        REQUIRE_FALSE(table.contains(10));
    }

    SECTION("contains() correctly finds multiple keys sharing the same bucket")
    {
        HashTable<int, int> table{1};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.contains(10));
        REQUIRE(table.contains(20));
        REQUIRE(table.contains(30));
        REQUIRE_FALSE(table.contains(40));
    }

    SECTION("contains() works with string keys")
    {
        HashTable<std::string, int> table;

        table.insert("Alice", 35);
        table.insert("Bob", 25);
        table.insert("Charlie", 30);

        REQUIRE(table.contains("Alice"));
        REQUIRE(table.contains("Bob"));
        REQUIRE(table.contains("Charlie"));
    }

    SECTION("contains() works on a const HashTable")
    {
        HashTable<int, int> table;
        table.insert(10, 100);

        const auto &const_table = table;

        REQUIRE(const_table.contains(10));
        REQUIRE_FALSE(const_table.contains(20));
    }
}

TEST_CASE("HashTable at() operations", "[HashTable]")
{
    SECTION("at() returns the correct value for existing key")
    {
        HashTable<int, int> table;

        table.insert(42, 35);
        table.insert(27, 15);

        REQUIRE(table.at(42) == 35);
        REQUIRE(table.at(27) == 15);
    }

    SECTION("at() throws for missing key")
    {
        HashTable<int, int> table;

        table.insert(42, 35);
        table.insert(27, 15);

        REQUIRE_THROWS_AS(table.at(10), std::out_of_range);
    }

    SECTION("at() throws for empty table")
    {
        HashTable<int, int> table;

        REQUIRE_THROWS_AS(table.at(10), std::out_of_range);
    }

    SECTION("at() returns a mutable reference")
    {
        HashTable<int, int> table;

        table.insert(42, 25);
        table.insert(27, 15);

        REQUIRE(table.at(42) == 25);
        REQUIRE(table.at(27) == 15);

        table.at(42) = 100;
        table.at(27) = 200;

        REQUIRE(table.at(42) == 100);
        REQUIRE(table.at(27) == 200);

        static_assert(std::is_same_v<
                      decltype(std::declval<const HashTable<int, int> &>().at(10)),
                      const int &>);
    }

    SECTION("at() works on a const HashTable")
    {
        HashTable<int, int> table;

        table.insert(42, 25);
        table.insert(27, 15);

        const auto &const_table = table;

        REQUIRE(const_table.at(42) == 25);
        REQUIRE(const_table.at(27) == 15);
    }

    SECTION("at() returns correct values for keys in the same bucket")
    {
        HashTable<int, int> table{1};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.at(10) == 100);
        REQUIRE(table.at(20) == 200);
        REQUIRE(table.at(30) == 300);
    }

    SECTION("at() works with string keys")
    {
        HashTable<std::string, int> table;

        table.insert("Alice", 25);
        table.insert("Bob", 20);
        table.insert("Charlie", 5);

        REQUIRE(table.at("Alice") == 25);
        REQUIRE(table.at("Bob") == 20);
        REQUIRE(table.at("Charlie") == 5);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Modifiers
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
TEST_CASE("HashTable insert() operations", "[HashTable]")
{
    SECTION("Inserting a single entry changes HashTable")
    {
        HashTable<int, int> table;

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE(table.bucket_count() == 8);

        REQUIRE(table.insert(42, 35));
        REQUIRE_FALSE(table.empty());
        REQUIRE(table.size() == 1);
    }

    SECTION("Inserting multiple entries changes HashTable")
    {
        HashTable<int, int> table;

        REQUIRE(table.insert(42, 35));
        REQUIRE(table.insert(16, 5));
        REQUIRE(table.insert(27, 15));
        REQUIRE(table.size() == 3);
    }

    SECTION("Inserting does not change HashTable for duplicate keys")
    {
        HashTable<int, int> table;

        REQUIRE(table.insert(42, 35));
        REQUIRE_FALSE(table.insert(42, 15));
        REQUIRE(table.size() == 1);
    }

    SECTION("Inserting colliding keys works")
    {
        HashTable<int, int> table{1};

        REQUIRE(table.insert(10, 100));
        REQUIRE(table.insert(20, 200));
        REQUIRE(table.insert(30, 300));

        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 1);
    }

    SECTION("Inserting string keys works")
    {
        HashTable<std::string, int> table;

        REQUIRE(table.insert("Alice", 25));
        REQUIRE(table.insert("Bob", 30));
        REQUIRE(table.insert("Charlie", 35));

        REQUIRE(table.size() == 3);

        REQUIRE_FALSE(table.insert("Alice", 40));
        REQUIRE(table.size() == 3);
    }
}
