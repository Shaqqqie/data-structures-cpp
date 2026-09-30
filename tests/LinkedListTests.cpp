#include <catch2/catch_test_macros.hpp>

#include "LinkedList.hpp"

#include <utility>
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

TEST_CASE("at() works correctly")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    REQUIRE(list.getSize() == 3);

    REQUIRE(list.at(0) == 10);
    REQUIRE(list.at(1) == 20);
    REQUIRE(list.at(2) == 30);
}

TEST_CASE("at() throws when index == size")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    REQUIRE(list.getSize() == 3);

    REQUIRE_THROWS_AS(list.at(list.getSize()), std::out_of_range);
}

TEST_CASE("at(0) on an empty list throws exception")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.at(0), std::out_of_range);
}

TEST_CASE("at() allows modifying values")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.at(1) = 100;

    REQUIRE(list.getSize() == 3);

    REQUIRE(list.at(0) == 10);
    REQUIRE(list.at(1) == 100);
    REQUIRE(list.at(2) == 30);
}

TEST_CASE("at() uses const overload for a const list")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    const LinkedList<int> &const_list{list};

    REQUIRE(const_list.at(1) == 20);
}

TEST_CASE("insert() modifies list for index(0), index(size), and index in the middle")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);

    list.insert(2, 25);

    REQUIRE(list.getSize() == 5);

    REQUIRE(list.at(0) == 10);
    REQUIRE(list.at(1) == 20);
    REQUIRE(list.at(2) == 25);
    REQUIRE(list.at(3) == 30);
    REQUIRE(list.at(4) == 40);

    list.insert(0, 5);
    REQUIRE(list.getSize() == 6);
    REQUIRE(list.front() == 5);

    list.insert(list.getSize(), 45);
    REQUIRE(list.getSize() == 7);
    REQUIRE(list.back() == 45);
}

TEST_CASE("insert() into an empty list")
{
    LinkedList<int> list;

    list.insert(0, 10);

    REQUIRE(list.getSize() == 1);

    REQUIRE(list.at(0) == 10);
}

TEST_CASE("insert() throws for index bigger than list size")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.insert(5, 500), std::out_of_range);
}

TEST_CASE("erase() works for first, last, and somewhere in between index")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);

    list.erase(0);
    REQUIRE(list.getSize() == 3);
    REQUIRE(list.front() == 20);

    list.erase(list.getSize() - 1);
    REQUIRE(list.getSize() == 2);
    REQUIRE(list.back() == 30);

    list.clear();
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);

    list.erase(2);
    REQUIRE(list.getSize() == 3);
    REQUIRE(list.at(1) == 20);
    REQUIRE(list.at(2) == 40);
}

TEST_CASE("erase() works for one-element list")
{
    LinkedList<int> list;
    list.push_back(10);

    list.erase(0);

    REQUIRE(list.empty());
    REQUIRE(list.getSize() == 0);
}

TEST_CASE("erase() throws for empty list and for invalid index")
{
    LinkedList<int> list;

    REQUIRE_THROWS_AS(list.erase(0), std::out_of_range);

    list.push_back(10);

    REQUIRE_THROWS_AS(list.erase(500), std::out_of_range);
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

TEST_CASE("Move constructor works correctly")
{
    LinkedList<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    LinkedList<int> moved{std::move(original)};

    REQUIRE(moved.getSize() == 3);
    REQUIRE(moved.front() == 10);
    REQUIRE(moved.back() == 30);

    REQUIRE(original.empty());
    REQUIRE(original.getSize() == 0);
}

TEST_CASE("Move assignment works")
{
    LinkedList<int> original;
    original.push_back(10);
    original.push_back(20);
    original.push_back(30);

    LinkedList<int> moved;
    moved.push_back(100);
    moved.push_back(200);

    moved = std::move(original);

    REQUIRE(moved.getSize() == 3);
    REQUIRE(moved.front() == 10);
    REQUIRE(moved.back() == 30);

    REQUIRE(original.empty());
    REQUIRE(original.getSize() == 0);
}

TEST_CASE("LinkedList handles self move assignment")
{
    LinkedList<int> list;
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list = std::move(list);

    REQUIRE(list.getSize() == 3);

    REQUIRE(list.front() == 10);
    REQUIRE(list.back() == 30);
}