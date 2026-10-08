#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "HashTable.hpp"

#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace
{
    struct ThrowOnCopy
    {
        int value;
        static inline bool should_throw = false;

        explicit ThrowOnCopy(int v)
            : value(v)
        {
        }

        ThrowOnCopy(const ThrowOnCopy &other)
            : value{other.value}
        {
           if (should_throw)
           {
            throw std::runtime_error("Copy failed");
           }
        }

        ThrowOnCopy(ThrowOnCopy &&other) noexcept = default;
        ThrowOnCopy &operator=(const ThrowOnCopy &) = default;
        ThrowOnCopy &operator=(ThrowOnCopy &&) noexcept = default;
    };
}

TEST_CASE("ThrowOnCopy direct exception diagnostic", "[ThrowOnCopy]")
{
    REQUIRE_THROWS_AS(
        throw std::runtime_error("Direct throw"),
        std::runtime_error);
}

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

TEST_CASE("HashTable erase() operations", "[HashTable]")
{
    SECTION("Erasing from an empty HashTable returns false")
    {
        HashTable<int, int> table;

        REQUIRE_FALSE(table.erase(10));
    }

    SECTION("Erasing an existing key returns true and decreases size_")
    {
        HashTable<int, int> table;

        table.insert(42, 15);
        table.insert(27, 25);
        table.insert(15, 123);

        REQUIRE(table.size() == 3);

        REQUIRE(table.erase(27));
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.contains(27));
    }

    SECTION("Erasing a nonexistent key returns false and leaves size_ unchanged")
    {
        HashTable<int, int> table;

        table.insert(42, 15);
        table.insert(27, 25);
        table.insert(15, 123);

        REQUIRE(table.size() == 3);

        REQUIRE_FALSE(table.erase(10));
        REQUIRE(table.size() == 3);
        REQUIRE_FALSE(table.contains(10));
    }

    SECTION("Erasing the first entry in a collision chain works")
    {
        HashTable<int, int> table{1};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.size() == 3);

        REQUIRE(table.erase(10));
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.contains(10));
    }

    SECTION("Erasing a middle entry in a collision chain works")
    {
        HashTable<int, int> table{1};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.size() == 3);

        REQUIRE(table.erase(20));
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.contains(20));
    }

    SECTION("Erasing the last entry in a collision chain works")
    {
        HashTable<int, int> table{1};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.size() == 3);

        REQUIRE(table.erase(30));
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.contains(30));
    }

    SECTION("Erasing the only entry leaves the HashTable empty")
    {
        HashTable<int, int> table;

        table.insert(42, 15);
        REQUIRE_FALSE(table.empty());
        REQUIRE(table.size() == 1);

        REQUIRE(table.erase(42));
        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE_FALSE(table.contains(42));
    }

    SECTION("Erasing a string key works")
    {
        HashTable<std::string, int> table;

        table.insert("Alice", 30);
        table.insert("Bob", 25);
        table.insert("Charlie", 5);

        REQUIRE(table.size() == 3);

        REQUIRE(table.erase("Bob"));
        REQUIRE(table.size() == 2);
        REQUIRE_FALSE(table.contains("Bob"));
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
        HashTable<int, int> table{};

        REQUIRE(table.insert(1, 100));
        REQUIRE(table.insert(9, 900));
        REQUIRE(table.insert(17, 1700));

        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 8);

        REQUIRE(table.at(1) == 100);
        REQUIRE(table.at(9) == 900);
        REQUIRE(table.at(17) == 1700);
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

TEST_CASE("HashTable clear() operations", "[HashTable]")
{
    SECTION("clear() on an empty HashTable leaves it empty")
    {
        HashTable<int, int> table;

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);

        table.clear();

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
    }

    SECTION("clear() removes all inserted entries")
    {
        HashTable<int, int> table;

        table.insert(42, 50);
        table.insert(10, 15);
        table.insert(27, 60);

        REQUIRE(table.size() == 3);
        REQUIRE_FALSE(table.empty());

        table.clear();

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE_FALSE(table.contains(42));
        REQUIRE_FALSE(table.contains(10));
        REQUIRE_FALSE(table.contains(27));
        REQUIRE(table.bucket_count() == 8);
    }

    SECTION("clear() works when multiple entries share a bucket")
    {
        HashTable<int, int> table{8};

        table.insert(1, 100);
        table.insert(9, 900);
        table.insert(17, 1700);

        REQUIRE_FALSE(table.empty());
        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 8);

        table.clear();

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE(table.bucket_count() == 8);
    }

    SECTION("Inserting new entries after calling clear() works")
    {
        HashTable<int, int> table;
        table.insert(42, 50);
        table.insert(10, 15);
        table.insert(27, 60);

        REQUIRE(table.size() == 3);

        table.clear();
        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);

        table.insert(10, 100);

        REQUIRE_FALSE(table.empty());
        REQUIRE(table.size() == 1);
        REQUIRE(table.contains(10));
    }

    SECTION("Calling clear() multiple times is safe")
    {
        HashTable<int, int> table;

        table.insert(10, 100);
        table.insert(20, 200);

        table.clear();
        table.clear();

        REQUIRE(table.empty());
        REQUIRE(table.size() == 0);
        REQUIRE(table.bucket_count() == 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// Rehashing
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

TEST_CASE("HashTable rehash() operations", "[HashTable]")
{
    SECTION("HashTable automatically rehashes when load factor exceeds maximum")
    {
        HashTable<int, int> table{4};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.bucket_count() == 4);
        REQUIRE(table.load_factor() == Catch::Approx(0.75f));

        table.insert(40, 400);

        REQUIRE(table.bucket_count() == 8);
        REQUIRE(table.size() == 4);
        REQUIRE(table.load_factor() == Catch::Approx(0.5f));

        REQUIRE(table.at(10) == 100);
        REQUIRE(table.at(20) == 200);
        REQUIRE(table.at(30) == 300);
        REQUIRE(table.at(40) == 400);
    }

    SECTION("HashTable handles repeated automatic rehashing")
    {
        HashTable<int, int> table{2};

        for (int i{0}; i < 100; ++i)
        {
            table.insert(i, i * 10);
        }

        REQUIRE(table.size() == 100);
        REQUIRE(table.bucket_count() == 256);
        REQUIRE(table.load_factor() <= table.max_load_factor());

        for (int i{0}; i < 100; ++i)
        {
            REQUIRE(table.at(i) == i * 10);
        }
    }

    SECTION("Duplicate insertion does not trigger rehashing")
    {
        HashTable<int, int> table{4};

        table.insert(10, 100);
        table.insert(20, 200);
        table.insert(30, 300);

        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 4);

        REQUIRE_FALSE(table.insert(20, 999));
        REQUIRE(table.at(20) == 200);
        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 4);
    }

    SECTION("Falied rehash preserved original entries")
    {
        HashTable<int, ThrowOnCopy> table{4};
        table.insert(10, ThrowOnCopy(100));
        table.insert(20, ThrowOnCopy(200));
        table.insert(30, ThrowOnCopy(300));

        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 4);
        REQUIRE(table.load_factor() == Catch::Approx(0.75f));

        ThrowOnCopy::should_throw = true;
        REQUIRE_THROWS_AS(table.insert(40, ThrowOnCopy(400)), std::runtime_error);

        ThrowOnCopy::should_throw = false;
        REQUIRE(table.size() == 3);
        REQUIRE(table.bucket_count() == 4);
        REQUIRE(table.at(10).value == 100);
        REQUIRE(table.at(20).value == 200);
        REQUIRE(table.at(30).value == 300);
    }

    SECTION("ThrowOnCopy throws when copied")
    {
        ThrowOnCopy original{100};

        ThrowOnCopy::should_throw = true;

        REQUIRE(ThrowOnCopy::should_throw);

        REQUIRE_THROWS_AS(
            [&]()
            {
                ThrowOnCopy copy{original};
                (void)copy;
            }(),
            std::runtime_error);

        ThrowOnCopy::should_throw = false;
    }
}
