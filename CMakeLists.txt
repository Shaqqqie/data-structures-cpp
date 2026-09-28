cmake_minimum_required(VERSION 3.20)

project(data_structures_cpp
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

add_library(data_structures INTERFACE)

target_include_directories(data_structures INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(data_structures INTERFACE
    -Wall
    -Wextra
    -Wpedantic
)

enable_testing()

add_executable(data_structures_tests 
    tests/LinkedListTests.cpp
)

target_link_libraries(data_structures_tests PRIVATE
    data_structures
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(data_structures_tests)