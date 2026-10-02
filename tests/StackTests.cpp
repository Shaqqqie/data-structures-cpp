#include <catch2/catch_test_macros.hpp>

#include "Stack.hpp"

#include <memory>
#include <stdexcept>
#include <type_traits>

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Construction / State
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
TEST_CASE("Stack is empty when constructed", "[Stack]")
{
    Stack<int> stack{};

    REQUIRE(stack.empty());
}

TEST_CASE("Stack size is zero when constructed", "[Stack]")
{
    Stack<int> stack{};

    REQUIRE(stack.size() == 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Stack can push elements", "[Stack]")
{
    Stack<int> stack;

    stack.push(20);

    REQUIRE(stack.size() == 1);
    REQUIRE_FALSE(stack.empty());

    stack.push(20);

    REQUIRE(stack.size() == 2);
}

TEST_CASE("Stack supports move-only elements", "[Stack]")
{
    Stack<std::unique_ptr<int>> stack{};

    auto ptr = std::make_unique<int>(42);

    stack.push(std::move(ptr));

    REQUIRE(ptr == nullptr);
    REQUIRE(stack.top());
    REQUIRE(*stack.top() == 42);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Push
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Stack top() returns the top element", "[Stack]")
{
    Stack<int> stack{};

    stack.push(10);
    stack.push(20);
    stack.push(30);

    REQUIRE(stack.top() == 30);
}

TEST_CASE("Stack top() returns a mutable reference", "[Stack]")
{
    Stack<int> stack{};

    stack.push(10);
    stack.push(20);
    stack.push(30);

    stack.top() = 100;

    REQUIRE(stack.top() == 100);

    static_assert(
        std::is_same_v<decltype(stack.top()), int &>);
}

TEST_CASE("Stack top() works with const stack", "[Stack]")
{
    Stack<int> stack{};

    stack.push(10);
    stack.push(20);
    stack.push(30);

    const Stack<int> &const_stack{stack};

    REQUIRE(const_stack.top() == 30);

    static_assert(
        std::is_same_v<decltype(const_stack.top()), const int &>);
}

TEST_CASE("Stack top() throws for empty stack", "[Stack]")
{
    Stack<int> stack{};

    REQUIRE_THROWS_AS(stack.top(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Pop
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Stack pop() removes elements in LIFO order", "[Stack]")
{
    Stack<int> stack{};

    stack.push(10);
    stack.push(20);
    stack.push(30);

    REQUIRE(stack.size() == 3);
    REQUIRE(stack.top() == 30);

    stack.pop();
    REQUIRE(stack.size() == 2);
    REQUIRE(stack.top() == 20);

    stack.pop();
    REQUIRE(stack.size() == 1);
    REQUIRE(stack.top() == 10);

    stack.pop();
    REQUIRE(stack.size() == 0);
    REQUIRE(stack.empty());
}

TEST_CASE("Stack pop() throws for empty stack", "[Stack]")
{
    Stack<int> stack{};

    REQUIRE_THROWS_AS(stack.pop(), std::out_of_range);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Copy Semantics
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Stack copy constructor creates independent copy", "[Stack]")
{
    Stack<int> original{};

    original.push(10);
    original.push(20);
    original.push(30);

    Stack<int> copy{original};

    REQUIRE(copy.size() == original.size());
    REQUIRE(copy.top() == original.top());

    copy.top() = 100;

    REQUIRE(copy.top() == 100);
    REQUIRE(original.top() == 30);
    

    copy.pop();

    REQUIRE(copy.size() == 2);
    REQUIRE(copy.top() == 20);

    REQUIRE(original.size() == 3);
    REQUIRE(original.top() == 30);
}

TEST_CASE("Stack copy assignment creates independent copy", "[Stack]")
{
    Stack<int> original{};

    original.push(10);
    original.push(20);

    Stack<int> copy{};
    copy.push(999);

    copy = original;

    REQUIRE(copy.size() == original.size());
    REQUIRE(copy.top() == original.top());

    copy.top() = 100;

    REQUIRE(copy.top() == 100);
    REQUIRE(original.top() == 20);

    copy.pop();

    REQUIRE(copy.size() == 1);
    REQUIRE(original.size() == 2);
}

TEST_CASE("Stack copy assignment handles self-assignment", "[Stack]")
{
    Stack<int> stack{};

    stack.push(10);
    stack.push(20);

    stack = stack;

    REQUIRE(stack.size() == 2);
    REQUIRE(stack.top() == 20);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Move Semantics
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Stack move constructor transfers contents", "[Stack]")
{
    Stack<int> original{};

    original.push(10);
    original.push(20);
    original.push(30);

    Stack<int> moved{std::move(original)};

    REQUIRE(moved.size() == 3);
    REQUIRE(moved.top() == 30);

    REQUIRE(original.empty());
    REQUIRE(original.size() == 0);

    original.push(100);

    REQUIRE(original.size() == 1);
    REQUIRE(original.top() == 100);
}

TEST_CASE("Stack move assignment transfers contents", "[Stack]")
{
    Stack<int> source{};
    source.push(10);
    source.push(20);
    source.push(30);

    Stack<int> destination{};
    destination.push(999);

    destination = std::move(source);

    REQUIRE(destination.size() == 3);
    REQUIRE(destination.top() == 30);

    REQUIRE(source.empty());
    REQUIRE(source.size() == 0);

    source.push(100);

    REQUIRE(source.size() == 1);
    REQUIRE(source.top() == 100);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Type Properties
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Stack supports expected copy and move semantics", "[Stack]")
{
    static_assert(std::is_copy_constructible_v<Stack<int>>);
    static_assert(std::is_copy_assignable_v<Stack<int>>);

    static_assert(std::is_move_constructible_v<Stack<int>>);
    static_assert(std::is_move_assignable_v<Stack<int>>);

    static_assert(std::is_nothrow_move_constructible_v<Stack<int>>);
    static_assert(std::is_nothrow_move_assignable_v<Stack<int>>);
}
