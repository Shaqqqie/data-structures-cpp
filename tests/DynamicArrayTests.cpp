#include <catch2/catch_test_macros.hpp>

#include "DynamicArray.hpp"

#include <string>
#include <utility>

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

TEST_CASE("DynamicArray at() accesses elements")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    REQUIRE(array.at(0) == 10);
    REQUIRE(array.at(1) == 20);
    REQUIRE(array.at(2) == 30);
}

TEST_CASE("DynamicArray at() throws for invalid index")
{
    DynamicArray<int> array;

    array.push_back(10);

    REQUIRE_THROWS_AS(array.at(1), std::out_of_range);
    REQUIRE_THROWS_AS(array.at(100), std::out_of_range);
}

TEST_CASE("DynamicArray can pop back elements")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    array.pop_back();

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 4);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);

    array.pop_back();
    REQUIRE(array.size() == 1);

    array.pop_back();
    REQUIRE(array.empty());

    // Should be safe on empty array
    array.pop_back();
    REQUIRE(array.empty());
}

TEST_CASE("DynamicArray can be cleared")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    array.clear();

    REQUIRE(array.size() == 0);
    REQUIRE(array.empty());
    REQUIRE(array.capacity() == 4);
}

TEST_CASE("DynamicArray can be copied")
{
    DynamicArray<int> original;

    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DynamicArray<int> copy{original};
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.capacity() == 4);
    REQUIRE(copy[0] == 10);
    REQUIRE(copy[1] == 20);
    REQUIRE(copy[2] == 30);

    copy[0] = 99;

    REQUIRE(copy[0] == 99);
    REQUIRE(original[0] == 10);

    copy.push_back(40);

    REQUIRE_THROWS_AS(original.at(3), std::out_of_range);
}

TEST_CASE("DynamicArray can be copied through assignment")
{
    DynamicArray<int> original;

    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DynamicArray<int> copy;
    copy.push_back(100);
    copy.push_back(200);

    copy = original;

    REQUIRE(copy.size() == 3);
    REQUIRE(copy.capacity() == 4);
    REQUIRE(copy[0] == 10);
    REQUIRE(copy[1] == 20);
    REQUIRE(copy[2] == 30);

    copy[0] = 99;

    REQUIRE(copy[0] == 99);
    REQUIRE(original[0] == 10);
}

TEST_CASE("DynamicArray handles self assignment")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    array = array;

    REQUIRE(array.size() == 3);
    REQUIRE(array.capacity() == 4);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
    REQUIRE(array[2] == 30);
}

TEST_CASE("DynamicArray can be move constructed")
{
    DynamicArray<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DynamicArray<int> moved{std::move(original)};
    REQUIRE(moved.size() == 3);
    REQUIRE(moved.capacity() == 4);
    REQUIRE(moved[0] == 10);
    REQUIRE(moved[1] == 20);
    REQUIRE(moved[2] == 30);

    REQUIRE(original.size() == 0);
    REQUIRE(original.capacity() == 0);
    REQUIRE(original.empty());
}

TEST_CASE("DynamicArray can be move assigned")
{
    DynamicArray<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    DynamicArray<int> destination;
    destination.push_back(100);
    destination.push_back(200);

    destination = std::move(original);

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.capacity() == 4);
    REQUIRE(destination[0] == 10);
    REQUIRE(destination[1] == 20);
    REQUIRE(destination[2] == 30);

    REQUIRE(original.empty());
    REQUIRE(original.size() == 0);
    REQUIRE(original.capacity() == 0);
}

TEST_CASE("DynamicArray Iterator can traverse elements")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    auto it = array.begin();

    REQUIRE(*it == 10);

    ++it;
    REQUIRE(*it == 20);

    ++it;
    REQUIRE(*it == 30);

    ++it;
    REQUIRE(it == array.end());
}

TEST_CASE("DynamicArray ConstIterator can traverse elements")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    const DynamicArray<int> &const_array{array};

    auto it = const_array.begin();

    REQUIRE(*it == 10);

    ++it;
    REQUIRE(*it == 20);

    ++it;
    REQUIRE(*it == 30);

    ++it;
    REQUIRE(it == const_array.end());
}

TEST_CASE("DynamicArray works with range-based for loop")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    int sum{0};

    for (int value : array)
    {
        sum += value;
    }

    REQUIRE(sum == 60);
}

TEST_CASE("DynamicArray front() returns first element and allows modification")
{
    DynamicArray<int> array;
    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    REQUIRE(array.front() == 10);

    array.front() = 100;

    REQUIRE(array.front() == 100);
    REQUIRE(array[0] == 100);
}

TEST_CASE("DynamicArray back() returns last element and allows modification")
{
    DynamicArray<int> array;
    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    REQUIRE(array.back() == 30);

    array.back() = 200;

    REQUIRE(array.back() == 200);
    REQUIRE(array[2] == 200);
}

TEST_CASE("DynamicArray front() throws when empty")
{
    DynamicArray<int> array;

    REQUIRE_THROWS_AS(array.front(), std::out_of_range);
}

TEST_CASE("DynamicArray back() throws when empty")
{
    DynamicArray<int> array;

    REQUIRE_THROWS_AS(array.back(), std::out_of_range);
}

TEST_CASE("DynamicArray reserve() increases capacity and preserves elements")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);

    array.reserve(10);

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 10);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
}

TEST_CASE("DynamicArray reserve() does not shrink capacity")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);

    array.reserve(10);

    array.reserve(5);

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 10);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
}

TEST_CASE("DynamicArray reserve() works on an empty array")
{
    DynamicArray<int> array;

    array.reserve(10);

    REQUIRE(array.size() == 0);
    REQUIRE(array.capacity() == 10);

    array.push_back(10);

    REQUIRE(array.size() == 1);
    REQUIRE(array.capacity() == 10);
    REQUIRE(array[0] == 10);
}

TEST_CASE("DynamicArray can grow beyond current capacity")
{
    DynamicArray<int> array;
    array.push_back(10);
    array.push_back(20);

    array.resize(5);

    REQUIRE(array.size() == 5);
    REQUIRE(array.capacity() == 5);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
    REQUIRE(array[2] == 0);
    REQUIRE(array[3] == 0);
    REQUIRE(array[4] == 0);
}

TEST_CASE("DynamicArray can grow within reserved capacity")
{
    DynamicArray<int> array;

    array.reserve(10);

    array.push_back(10);
    array.push_back(20);

    array.resize(5);

    REQUIRE(array.size() == 5);
    REQUIRE(array.capacity() == 10);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
    REQUIRE(array[2] == 0);
    REQUIRE(array[3] == 0);
    REQUIRE(array[4] == 0);
}

TEST_CASE("DynamicArray can shrink in size")
{
    DynamicArray<int> array;

    array.push_back(10);
    array.push_back(20);
    array.push_back(30);
    array.push_back(40);
    array.push_back(50);

    REQUIRE(array.size() == 5);
    REQUIRE(array.capacity() == 8);

    array.resize(2);

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 8);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
}

TEST_CASE("DynamicArray resize() also works for other types than int")
{
    DynamicArray<std::string> array;

    array.push_back("hello");
    array.push_back("world");

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 2);

    array.resize(4);

    REQUIRE(array.size() == 4);
    REQUIRE(array.capacity() == 4);
    REQUIRE(array[0] == "hello");
    REQUIRE(array[1] == "world");
    REQUIRE(array[2] == "");
    REQUIRE(array[3] == "");
}

TEST_CASE("DynamicArray data() provides mutable access to underlying storage")
{
    DynamicArray<int> array;
    array.push_back(10);
    array.push_back(20);
    array.push_back(30);

    int *ptr = array.data();

    REQUIRE(ptr[0] == 10);
    REQUIRE(ptr[1] == 20);
    REQUIRE(ptr[2] == 30);

    ptr[2] = 40;

    REQUIRE(array[2] == 40);
}

TEST_CASE("DynamicArray data() provides const access to underlying storage")
{
    DynamicArray<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    const DynamicArray<int> array{original};

    const int *ptr = array.data();

    REQUIRE(ptr[0] == 10);
    REQUIRE(ptr[1] == 20);
    REQUIRE(ptr[2] == 30);
}

TEST_CASE("DynamicArray shrink_to_fit shrinks capacity to size")
{
    DynamicArray<int> array;
    array.push_back(10);
    array.push_back(20);

    array.reserve(10);

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 10);

    array.shrink_to_fit();

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 2);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
}

TEST_CASE("DynamicArray shrink_to_fit does nothing when size equals capacity")
{
    DynamicArray<int> array;
    array.push_back(10);
    array.push_back(20);

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 2);

    array.shrink_to_fit();

    REQUIRE(array.size() == 2);
    REQUIRE(array.capacity() == 2);
    REQUIRE(array[0] == 10);
    REQUIRE(array[1] == 20);
}

TEST_CASE("DynamicArray shrink_to_fit releases storage for empty array")
{
    DynamicArray<int> array;
    array.reserve(10);

    array.shrink_to_fit();

    REQUIRE(array.size() == 0);
    REQUIRE(array.capacity() == 0);
    REQUIRE(array.empty());

    REQUIRE(array.data() == nullptr);
}