# Data Structures in C++

A collection of data structures implemented from scratch in modern C++.

This project is focused on understanding how common data structures work internally, while practicing manual memory management, ownership, templates, iterators, testing, and modern C++ techniques.

The goal is not to replace the C++ Standard Library, but to build the underlying structures myself for learning purposes.

## Implemented Data Structures

### Linked List

A templated singly linked list implemented using dynamically allocated nodes.

Currently supports:

- `push_front()`
- `push_back()`
- `pop_front()`
- `pop_back()`
- `insert()`
- `erase()`
- `clear()`
- `at()`
- `front()`
- `back()`
- `contains()`
- `find()`
- `empty()`
- Size tracking
- Bounds checking
- Copy construction
- Copy assignment
- Move construction
- Move assignment
- Custom iterator
- Custom const iterator
- Range-based `for` loops

The implementation manages node lifetime manually using `new` and `delete`.

### Dynamic Array

A templated dynamically allocated array inspired by the basic behavior of `std::vector`.

Currently supports:

- `push_back()`
- `operator[]`
- Const `operator[]`
- Automatic capacity growth
- Size tracking
- Capacity tracking
- Empty-state checking
- Dynamic allocation and deallocation

The array automatically grows when its current capacity is exhausted.

The current growth strategy doubles the capacity:

```text
0 -> 1 -> 2 -> 4 -> 8 -> ...
```

`DynamicArray` is still under development. Additional element-access and modifier operations will be added as the project progresses.

## Technologies

- **C++20**
- **CMake**
- **Catch2**
- **GitHub Actions**
- GCC / Clang
- AddressSanitizer and UndefinedBehaviorSanitizer

## Project Structure

```text
data-structures-cpp/
├── include/
│   ├── DynamicArray.hpp
│   └── LinkedList.hpp
├── tests/
│   ├── DynamicArrayTests.cpp
│   └── LinkedListTests.cpp
├── src/
├── CMakeLists.txt
└── README.md
```

Because the data structures are templated, their implementations are currently kept in their header files.

## Building the Project

### Requirements

- A C++20-compatible compiler
- CMake
- Git

### Clone

```bash
git clone https://github.com/Shaqqqie/data-structures-cpp.git
cd data-structures-cpp
```

### Configure

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

## Running the Tests

The project uses Catch2 for automated testing.

```bash
ctest --test-dir build --output-on-failure
```

The test suite covers functionality such as:

- Construction and empty states
- Insertion and removal
- Element access
- Invalid indices
- Copy semantics
- Move semantics
- Self-assignment
- Searching
- Iterator traversal
- Range-based `for` loops
- Const iteration
- Dynamic-array growth
- Element modification

## Memory Safety

Because these data structures manage memory manually, memory safety is an important part of the project.

Sanitizer builds can be used to detect issues such as:

- Memory leaks
- Invalid memory access
- Use-after-free
- Undefined behavior

This is especially useful for testing operations involving dynamically allocated linked-list nodes and dynamic arrays.

## What I Am Practicing

This project is primarily intended to strengthen my understanding of:

- Data structures
- Pointers
- Dynamic memory allocation
- Object lifetime
- Resource ownership
- Copy semantics
- Move semantics
- Templates
- Iterators
- Const correctness
- Exception handling
- Algorithmic complexity
- Unit testing
- Debugging memory-related problems

## Why Implement Standard Data Structures?

In normal C++ applications, Standard Library containers such as `std::vector` and `std::list` should generally be preferred.

The purpose of this project is different.

Implementing these structures manually makes it possible to explore what happens underneath those abstractions, including:

- How nodes are linked using pointers
- How dynamically allocated memory is acquired and released
- How containers grow their storage
- How ownership affects copy and move operations
- How iterators provide an interface for traversal
- How edge cases can lead to memory errors or undefined behavior

## Planned Data Structures

As the project progresses, I plan to implement additional structures such as:

- Stack
- Queue
- Binary Search Tree
- AVL Tree
- Hash Table
- Graph

Existing implementations will also continue to be expanded and tested.

## Status

This is an active learning project and is intentionally being developed incrementally.

Each data structure is implemented and tested separately so that I can focus on understanding its design, memory behavior, operations, and complexity before moving on to the next one.