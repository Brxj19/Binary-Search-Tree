# Getting Started

## Requirements

- A C++17 compiler
- No external dependencies

## Install by copying the header

Copy the `include/` directory into your project and compile with that directory on your include path.

```cpp
#include <bst/bst.h>
```

Example compile command:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic your_program.cpp -I/path/to/include -o your_program
```

## Install with Git submodule

```bash
git submodule add https://github.com/Brxj19/Binary-Search-Tree.git external/Binary-Search-Tree
```

Then include the header and point your compiler at `external/Binary-Search-Tree/include`.

## First example

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

## Build examples and tests with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

## Direct test build

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic tests/test_bst.cpp -Iinclude -o test_bst
./test_bst
```

## Notes

- The tree stores unique values only.
- Ordering is controlled by the `Compare` template parameter.
- This is a normal Binary Search Tree, not an AVL tree or Red-Black tree.
