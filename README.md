# BST.h: A Modern C++17 Binary Search Tree

[![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

`BST.h` is a modern, header-only C++17 library providing a node-based binary search tree. It is built with modern C++ practices, including smart pointers for automatic memory management (`std::unique_ptr`), move semantics, and emplacement for optimal performance.

This library is ideal for scenarios where you need fast insertion, deletion, and lookup of sorted data.

## `Documentation`
Online Documentation of [BST.h Docs ](https://brxj19.github.io/Binary-Search-Tree/)


## Features

-   **Automatic Memory Management**: No manual `new` or `delete` required, thanks to `std::unique_ptr`.
-   **Sorted Order**: Elements are always kept in sorted order automatically.
-   **In-place Construction**: `emplace` functions construct objects directly in the tree, avoiding copies.
-   **STL-like Interface**: Follows standard library conventions for methods like `insert`, `find`, `begin`, `end`, etc.
-   **Bidirectional Iterators**: Allows for forward and backward traversal through the sorted elements.
-   **Build from Traversals**: Reconstruct a tree from pre-order/in-order or post-order/in-order traversals.

## Getting Started

### Prerequisites

A C++ compiler that supports the **C++17 standard** or newer (e.g., GCC 7+, Clang 5+).

### Usage

Since this is a header-only library, there is no need to build it separately. Simply include [`bst.h`](bst.h) in your project and compile your source files with the C++17 standard enabled.

#### Example compilation command:

```bash
g++ -std=c++17 your_main_file.cpp -o your_program
```

## Basic Usage

```cpp
#include <iostream>
#include "bst.h"

int main() {
    // Create a tree using an initializer list
    BinarySearchTree<int> bst = {50, 30, 70, 20, 40};

    // Add an element
    bst.insert(80);

    // Iterate and print elements (always in sorted order)
    std::cout << "Tree contents: ";
    for (const auto& val : bst) {
        std::cout << val << " ";
    }
    // Output: 20 30 40 50 70 80
    std::cout << std::endl;

    // Check for an element
    if (bst.contains(40)) {
        std::cout << "The tree contains the value 40." << std::endl;
    }

    // Remove an element
    bst.erase(30);

    return 0;
}
```

## API Overview

A brief overview of the provided API. For detailed examples, see [`index.html`](index.html).

-   **Constructors**: Default, copy, move, initializer list, and from traversals.
-   **Capacity**: `size()`, `empty()`
-   **Modifiers**: `insert()`, `emplace()`, `erase()`, `clear()`
-   **Lookup**: `find()`, `contains()`
-   **Iterators**: `begin()`, `end()`, `cbegin()`, `cend()`
-   **Traversals**: `in_order_traversal()`, `pre_order_traversal()`, `post_order_traversal()`

## Performance

Performance depends on the height of the tree. For a balanced tree, operations are logarithmic. In the worst case (an unbalanced tree), they are linear.

| Operation         | Average Case (Balanced) | Worst Case (Unbalanced) |
| ----------------- | ----------------------- | ----------------------- |
| Access / Search   | O(log N)                | O(N)                    |
| Insertion         | O(log N)                | O(N)                    |
| Deletion          | O(log N)                | O(N)                    |
| `size()`, `empty()` | O(1)                    | O(1)                    |

## Building and Running Tests

The project includes a test suite in [`test.cpp`](test.cpp). To build and run the tests:

```bash
# Compile the tests
g++ -std=c++17 test.cpp -o test

# Run the tests
./test
```

On success, the output will be: `All BinarySearchTree tests passed successfully!`
