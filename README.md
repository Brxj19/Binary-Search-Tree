# BinarySearchTree.h

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Header-only](https://img.shields.io/badge/library-header--only-success.svg)](#installation)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![CI](https://github.com/Brxj19/Binary-Search-Tree/actions/workflows/ci.yml/badge.svg)](https://github.com/Brxj19/Binary-Search-Tree/actions/workflows/ci.yml)

A modern header-only C++17 Binary Search Tree library with smart-pointer memory management, STL-style iterators, traversal utilities, comparator support, and detailed documentation.

## Features

- Header-only C++17 library
- Smart-pointer-based memory management with `std::unique_ptr`
- `insert`, `emplace`, `erase`, `find`, `contains`, `clear`
- STL-style iterators and const iterators
- Custom comparator support with `BinarySearchTree<T, Compare>`
- Traversal helpers for in-order, pre-order, and post-order visits
- Copy and move support
- `lower_bound`, `upper_bound`, `min`, `max`, `height`, `to_vector`, `is_valid_bst`
- Simple examples, tests, CMake support, and GitHub Actions CI

## Important note

This is a plain Binary Search Tree, not a self-balancing AVL or Red-Black tree. For sorted input, operations can degrade to `O(N)`.

## Installation

Download the repository and add the `include/` directory to your include path.

```cpp
#include <bst/bst.h>
```

A small root-level `bst.h` wrapper is also kept in the repository for backwards compatibility.

### Git submodule

```bash
git submodule add https://github.com/Brxj19/Binary-Search-Tree.git external/Binary-Search-Tree
```

Then compile with `external/Binary-Search-Tree/include` on the include path.

## Quick example

```cpp
#include <iostream>
#include <bst/bst.h>

int main() {
    BinarySearchTree<int> tree = {8, 3, 10, 1, 6};

    tree.insert(14);
    tree.erase(3);

    for (int value : tree) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}
```

## Build and test

### CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

### Direct `g++`

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic tests/test_bst.cpp -Iinclude -o test_bst
./test_bst
```

## Complexity

| Operation | Average | Worst |
| --- | --- | --- |
| Search | `O(log N)` | `O(N)` |
| Insert | `O(log N)` | `O(N)` |
| Delete | `O(log N)` | `O(N)` |
| Traversal | `O(N)` | `O(N)` |

## Documentation

GitHub Pages documentation: https://brxj19.github.io/Binary-Search-Tree/

Repository docs:

- [Overview](docs/index.md)
- [Getting Started](docs/getting-started.md)
- [API Reference](docs/api-reference.md)
- [Examples](docs/examples.md)
- [Performance](docs/performance.md)

## Project structure

```text
Binary-Search-Tree/
├── include/bst/bst.h
├── examples/
├── tests/test_bst.cpp
├── docs/
├── .github/workflows/ci.yml
├── CMakeLists.txt
├── README.md
├── LICENSE
├── CHANGELOG.md
└── index.html
```

## License

MIT License. See [LICENSE](LICENSE).
