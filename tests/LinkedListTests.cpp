#include <catch2/catch_test_macros.hpp>

#include "LinkedList.hpp"

#include <utility>
TEST_CASE("Testing default values for newly created LinkedList")
{
    LinkedList<int> list;

    REQUIRE(list.empty());
    REQUIRE(list.getSize() == 0);
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

TEST_CASE("Move constructor works correctly")
{
    LinkedList<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    LinkedList<int> moved{std::move(original)};

    LinkedList<int> empty;
    REQUIRE(empty.empty());
    REQUIRE_FALSE(empty.contains(10));
}

TEST_CASE("contains() works for const lists")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    const LinkedList<int> &const_list{list};

    REQUIRE(const_list.getSize() == 3);
    REQUIRE(const_list.contains(10));
    REQUIRE(const_list.contains(20));
    REQUIRE(const_list.contains(30));
    REQUIRE_FALSE(const_list.contains(99));
}

TEST_CASE("find() returns index of first occurrence")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    std::size_t index{};
    for (const auto value : list)
    {
        auto result = list.find(value);

        REQUIRE(result.has_value());
        REQUIRE(result.value() == index);

        ++index;
    }

    auto invalid_result = list.find(99);
    REQUIRE_FALSE(invalid_result.has_value());
}

TEST_CASE("Iterator works correctly for traversing a list")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    auto it = list.begin();
    REQUIRE(*it == list.front());
    REQUIRE(it != list.end());

    ++it;
    REQUIRE(*it == list.at(1));
    REQUIRE(it != list.end());

    ++it;
    REQUIRE(*it == list.at(2));
    REQUIRE(it != list.end());

    ++it;
    REQUIRE(it == list.end());
}

TEST_CASE("begin() and end() are equal for empty list")
{
    LinkedList<int> list;

    REQUIRE(list.begin() == list.end());
}

TEST_CASE("LinkedList supports range-based for loop")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    std::size_t index{};
    for (int value : list)
    {
        REQUIRE(value == list.at(index));
        ++index;
    }

    REQUIRE(index == list.getSize());
}

TEST_CASE("LinkedList can modify data through range-based for loop")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    for (int &value : list)
    {
        value *= 2;
    }

    REQUIRE(list.at(0) == 20);
    REQUIRE(list.at(1) == 40);
    REQUIRE(list.at(2) == 60);

    REQUIRE(list.getSize() == 3);
}

TEST_CASE("Const LinkedList supports range-based for loop")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    const LinkedList<int> &const_list{list};

    std::size_t index{};
    for (const int &value : const_list)
    {
        REQUIRE(value == const_list.at(index));
        ++index;
    }

    REQUIRE(index == const_list.getSize());
}

TEST_CASE("LinkedList can insert at the beginning")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.insert(0, 5);

    REQUIRE(list.getSize() == 4);
    REQUIRE(list.at(0) == 5);
}

TEST_CASE("LinkedList can insert in middle")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.insert(1, 15);

    REQUIRE(list.getSize() == 4);
    REQUIRE(list.at(0) == 10);
    REQUIRE(list.at(1) == 15);
    REQUIRE(list.at(2) == 20);
    REQUIRE(list.at(3) == 30);
}

TEST_CASE("LinkedList can insert at end")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.insert(3, 40);

    REQUIRE(list.getSize() == 4);
    REQUIRE(list.at(3) == 40);
}

TEST_CASE("LinkedList insert rejects invalid index")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    REQUIRE_THROWS_AS(list.insert(4, 40), std::out_of_range);
}

TEST_CASE("LinkedList can erase first element")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.erase(0);

    REQUIRE(list.getSize() == 2);
    REQUIRE(list.front() == 20);
    REQUIRE(list.back() == 30);
}

TEST_CASE("LinkedList can erase middle element")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.erase(1);

    REQUIRE(list.getSize() == 2);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 30);
}

TEST_CASE("LinkedList can erase last element")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.erase(2);

    REQUIRE(list.getSize() == 2);
    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 20);
}

TEST_CASE("LinkedList erase rejects invalid index")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    REQUIRE_THROWS_AS(list.erase(4), std::out_of_range);
}

