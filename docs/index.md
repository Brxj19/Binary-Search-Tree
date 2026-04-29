# BinarySearchTree Documentation

`BinarySearchTree` is a header-only C++17 Binary Search Tree container designed to stay readable, reusable, and easy to learn from. It offers STL-style insertion and lookup, bidirectional iterators, custom comparator support, traversal helpers, and smart-pointer-based ownership.

This library is a plain Binary Search Tree. It is not self-balancing, so sorted or nearly sorted insertion orders can degrade search, insertion, and erase from average `O(log N)` behavior to worst-case `O(N)`.

## Documentation map

- [Getting Started](getting-started.md)
- [API Reference](api-reference.md)
- [Examples](examples.md)
- [Performance Notes](performance.md)

## Published HTML site

- [Documentation home](../index.html)
- [Published overview page](index.html)
- [Published API reference](api-reference.html)

## Highlights

- Header-only and C++17 compatible
- `std::unique_ptr`-based node ownership
- `insert`, `emplace`, `find`, `contains`, `erase`, `clear`
- Bidirectional iterators and const iterators
- `lower_bound`, `upper_bound`, `min`, `max`, `height`, `to_vector`
- Traversal helpers for in-order, pre-order, and post-order visits
- Copy and move support

## Include path

```cpp
#include <bst/bst.h>
```

For backwards compatibility inside this repository, a small root-level `bst.h` wrapper is also provided.
