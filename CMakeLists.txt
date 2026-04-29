cmake_minimum_required(VERSION 3.14)

project(BinarySearchTree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(BinarySearchTree INTERFACE)
add_library(BinarySearchTree::BinarySearchTree ALIAS BinarySearchTree)

target_include_directories(BinarySearchTree
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(BinarySearchTree INTERFACE cxx_std_17)

add_executable(bst_tests tests/test_bst.cpp)
target_link_libraries(bst_tests PRIVATE BinarySearchTree::BinarySearchTree)
target_compile_options(bst_tests PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_test(NAME BinarySearchTreeTests COMMAND bst_tests)

add_executable(example_basic_usage examples/basic_usage.cpp)
target_link_libraries(example_basic_usage PRIVATE BinarySearchTree::BinarySearchTree)
target_compile_options(example_basic_usage PRIVATE -Wall -Wextra -Wpedantic)

add_executable(example_custom_object examples/custom_object.cpp)
target_link_libraries(example_custom_object PRIVATE BinarySearchTree::BinarySearchTree)
target_compile_options(example_custom_object PRIVATE -Wall -Wextra -Wpedantic)

add_executable(example_iterator_usage examples/iterator_usage.cpp)
target_link_libraries(example_iterator_usage PRIVATE BinarySearchTree::BinarySearchTree)
target_compile_options(example_iterator_usage PRIVATE -Wall -Wextra -Wpedantic)

add_executable(example_traversal_usage examples/traversal_usage.cpp)
target_link_libraries(example_traversal_usage PRIVATE BinarySearchTree::BinarySearchTree)
target_compile_options(example_traversal_usage PRIVATE -Wall -Wextra -Wpedantic)
