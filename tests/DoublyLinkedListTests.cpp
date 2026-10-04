#include <catch2/catch_test_macros.hpp>

#include "DoublyLinkedList.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

struct ThrowOnCopy
{
    int value;

    static inline int copy_count{0};
    static inline int throw_after{-1};

    explicit ThrowOnCopy(int value)
        : value{value}
    {
    }

    ThrowOnCopy(const ThrowOnCopy &other)
        : value{other.value}
    {
        if (throw_after >= 0 && copy_count++ >= throw_after)
        {
            throw std::runtime_error("Copy failed.");
        }
    }

    ThrowOnCopy(ThrowOnCopy &&other) noexcept
        : value{other.value}
    {
    }

    static void reset(int after = -1)
    {
        copy_count = 0;
        throw_after = after;
    }
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / Empty State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList is empty after construction", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push Front
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList push_front() adds elements", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_front(10);
    list.push_front(20);
    list.push_front(30);

    REQUIRE_FALSE(list.empty());
    REQUIRE(list.size() == 3);
}

TEST_CASE("DoublyLinkedList push_front() maintains correct order", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_front(10);
    list.push_front(20);
    list.push_front(30);

    REQUIRE(list.front() == 30);
    REQUIRE(list.back() == 10);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push Back
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList push_back() adds elements", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    REQUIRE_FALSE(list.empty());
    REQUIRE(list.size() == 3);
}

TEST_CASE("DoublyLinkedList push_back() maintains correct order", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Mixed Push Operations
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList supports push_front() and push_back() together", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(20);
    list.push_front(10);
    list.push_back(30);

    REQUIRE(list.size() == 3);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 30);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Front / Back
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList front() returns first element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    REQUIRE(list.front() == 10);
}

TEST_CASE("DoublyLinkedList back() returns last element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList front() returns modifiable reference", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);

    list.front() = 100;

    REQUIRE(list.front() == 100);
}

TEST_CASE("DoublyLinkedList const front() returns first element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    const DoublyLinkedList<int> &const_list{list};

    REQUIRE(const_list.front() == 10);
}

TEST_CASE("DoublyLinkedList const back() returns last element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    const DoublyLinkedList<int> &const_list{list};

    REQUIRE(const_list.back() == 20);
}

TEST_CASE("DoublyLinkedList front() throws when empty", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    REQUIRE_THROWS_AS(list.front(), std::out_of_range);
}

TEST_CASE("DoublyLinkedList back() throws when empty", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    REQUIRE_THROWS_AS(list.back(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Pop Front
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList pop_front() removes first element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_front();

    REQUIRE(list.size() == 2);
    REQUIRE(list.front() == 20);
    REQUIRE(list.back() == 30);
}

TEST_CASE("DoublyLinkedList pop_front() handles single element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.pop_front();

    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
}

TEST_CASE("DoublyLinkedList can be reused after pop_front() removes last element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.pop_front();

    list.push_back(20);

    REQUIRE_FALSE(list.empty());
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == 20);
    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList pop_front() throws when empty", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    REQUIRE_THROWS_AS(list.pop_front(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Pop Back
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList pop_back() removes last element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_back();

    REQUIRE(list.size() == 2);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList pop_back() handles single element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.pop_back();

    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
}

TEST_CASE("DoublyLinkedList can be reused after pop_back() removes last element", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.pop_back();

    list.push_front(20);

    REQUIRE_FALSE(list.empty());
    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == 20);
    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList pop_back() throws when empty", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    REQUIRE_THROWS_AS(list.pop_back(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Forward / Backward Link Integrity
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList maintains links after pop_front()", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_front();
    list.pop_back();

    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == 20);
    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList maintains links after pop_back()", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_back();

    list.push_back(40);

    REQUIRE(list.size() == 3);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 40);

    list.pop_back();

    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList handles alternating operations at both ends", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_front(20);
    list.push_front(10);
    list.push_back(30);
    list.push_back(40);

    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 40);
    REQUIRE(list.size() == 4);

    list.pop_front();

    REQUIRE(list.front() == 20);

    list.pop_back();

    REQUIRE(list.back() == 30);

    list.pop_front();

    REQUIRE(list.front() == 30);
    REQUIRE(list.back() == 30);

    list.pop_back();

    REQUIRE(list.empty());
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Clear
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList clear() removes all elements", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.clear();

    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
}

TEST_CASE("DoublyLinkedList clear() works on empty list", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.clear();

    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
}

TEST_CASE("DoublyLinkedList can be reused after clear()", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    list.clear();

    list.push_front(30);
    list.push_back(40);

    REQUIRE(list.size() == 2);
    REQUIRE(list.front() == 30);
    REQUIRE(list.back() == 40);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy Constructor
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList copy constructor creates independent copy", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> original;

    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DoublyLinkedList<int> copy{original};

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.front() == 10);
    REQUIRE(copy.back() == 30);

    copy.front() = 100;
    copy.back() = 300;

    REQUIRE(original.front() == 10);
    REQUIRE(original.back() == 30);
    REQUIRE(copy.front() == 100);
    REQUIRE(copy.back() == 300);
}

TEST_CASE("DoublyLinkedList copy constructor handles empty list", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> original;

    DoublyLinkedList<int> copy{original};

    REQUIRE(copy.empty());
    REQUIRE(copy.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy Assignment
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList copy assignment creates independent copy", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> original;

    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DoublyLinkedList<int> copy;

    copy.push_back(100);
    copy.push_back(200);

    copy = original;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.front() == 10);
    REQUIRE(copy.back() == 30);

    copy.front() = 1000;
    copy.back() = 3000;

    REQUIRE(original.front() == 10);
    REQUIRE(original.back() == 30);
}

TEST_CASE("DoublyLinkedList copy assignment handles self-assignment", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    list = list;

    REQUIRE(list.size() == 2);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList copy assignment from empty list", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> source;

    DoublyLinkedList<int> destination;

    destination.push_back(10);
    destination.push_back(20);

    destination = source;

    REQUIRE(destination.empty());
    REQUIRE(destination.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Constructor
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList move constructor transfers ownership", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> original;

    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DoublyLinkedList<int> moved{std::move(original)};

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.front() == 10);
    REQUIRE(moved.back() == 30);

    REQUIRE(original.empty());
    REQUIRE(original.size() == 0);
}

TEST_CASE("DoublyLinkedList moved-from object can be reused after move construction", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> original;

    original.push_back(10);

    DoublyLinkedList<int> moved{std::move(original)};

    original.push_back(20);

    REQUIRE(original.size() == 1);
    REQUIRE(original.front() == 20);
    REQUIRE(original.back() == 20);

    REQUIRE(moved.size() == 1);
    REQUIRE(moved.front() == 10);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Assignment
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList move assignment transfers ownership", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> source;

    source.push_back(10);
    source.push_back(20);
    source.push_back(30);

    DoublyLinkedList<int> destination;

    destination.push_back(100);
    destination.push_back(200);

    destination = std::move(source);

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.front() == 10);
    REQUIRE(destination.back() == 30);

    REQUIRE(source.empty());
    REQUIRE(source.size() == 0);
}

TEST_CASE("DoublyLinkedList move assignment handles self-assignment", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> list;

    list.push_back(10);
    list.push_back(20);

    auto &same_list{list};

    list = std::move(same_list);

    REQUIRE(list.size() == 2);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 20);
}

TEST_CASE("DoublyLinkedList moved-from object can be reused after move assignment", "[DoublyLinkedList]")
{
    DoublyLinkedList<int> source;

    source.push_back(10);

    DoublyLinkedList<int> destination;

    destination = std::move(source);

    source.push_front(20);

    REQUIRE(source.size() == 1);
    REQUIRE(source.front() == 20);
    REQUIRE(source.back() == 20);

    REQUIRE(destination.size() == 1);
    REQUIRE(destination.front() == 10);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move-Only Types
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList push_front() supports move-only types", "[DoublyLinkedList]")
{
    DoublyLinkedList<std::unique_ptr<int>> list;

    list.push_front(std::make_unique<int>(10));
    list.push_front(std::make_unique<int>(20));

    REQUIRE(list.size() == 2);
    REQUIRE(*list.front() == 20);
    REQUIRE(*list.back() == 10);
}

TEST_CASE("DoublyLinkedList push_back() supports move-only types", "[DoublyLinkedList]")
{
    DoublyLinkedList<std::unique_ptr<int>> list;

    list.push_back(std::make_unique<int>(10));
    list.push_back(std::make_unique<int>(20));

    REQUIRE(list.size() == 2);
    REQUIRE(*list.front() == 10);
    REQUIRE(*list.back() == 20);
}

TEST_CASE("DoublyLinkedList transfers ownership of moved value", "[DoublyLinkedList]")
{
    DoublyLinkedList<std::unique_ptr<int>> list;

    auto value = std::make_unique<int>(42);

    list.push_back(std::move(value));

    REQUIRE(value == nullptr);
    REQUIRE(list.size() == 1);
    REQUIRE(*list.front() == 42);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Different Value Types
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList works with strings", "[DoublyLinkedList]")
{
    DoublyLinkedList<std::string> list;

    list.push_back("middle");
    list.push_front("first");
    list.push_back("last");

    REQUIRE(list.size() == 3);
    REQUIRE(list.front() == "first");
    REQUIRE(list.back() == "last");

    list.pop_front();
    list.pop_back();

    REQUIRE(list.size() == 1);
    REQUIRE(list.front() == "middle");
    REQUIRE(list.back() == "middle");
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Exception Handling
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("DoublyLinkedList copy constructor cleans up when element copy throws", "[DoublyLinkedList]")
{
    ThrowOnCopy::reset();

    DoublyLinkedList<ThrowOnCopy> original;

    original.push_back(ThrowOnCopy{10});
    original.push_back(ThrowOnCopy{20});
    original.push_back(ThrowOnCopy{30});

    ThrowOnCopy::reset(1);

    REQUIRE_THROWS_AS(
        DoublyLinkedList<ThrowOnCopy>{original},
        std::runtime_error);

    REQUIRE(original.size() == 3);
    REQUIRE(original.front().value == 10);
    REQUIRE(original.back().value == 30);
}

TEST_CASE("DoublyLinkedList copy assignment preserves destination when element copy throws", "[DoublyLinkedList]")
{
    ThrowOnCopy::reset();

    DoublyLinkedList<ThrowOnCopy> source;

    source.push_back(ThrowOnCopy{10});
    source.push_back(ThrowOnCopy{20});
    source.push_back(ThrowOnCopy{30});

    DoublyLinkedList<ThrowOnCopy> destination;

    destination.push_back(ThrowOnCopy{100});
    destination.push_back(ThrowOnCopy{200});

    ThrowOnCopy::reset(1);

    REQUIRE_THROWS_AS(
        destination = source,
        std::runtime_error);

    REQUIRE(destination.size() == 2);
    REQUIRE(destination.front().value == 100);
    REQUIRE(destination.back().value == 200);

    REQUIRE(source.size() == 3);
    REQUIRE(source.front().value == 10);
    REQUIRE(source.back().value == 30);
}