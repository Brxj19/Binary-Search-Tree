# Examples

The repository includes buildable examples in the [`examples/`](../examples) directory.

## Basic usage

Source: [`examples/basic_usage.cpp`](../examples/basic_usage.cpp)

```cpp
BinarySearchTree<int> tree = {8, 3, 10, 1, 6, 14, 4, 7, 13};
tree.insert(5);
tree.erase(10);

for (int value : tree) {
    std::cout << value << ' ';
}
```

## Custom objects and comparators

Source: [`examples/custom_object.cpp`](../examples/custom_object.cpp)

```cpp
struct BookTitleLess {
    bool operator()(const Book& lhs, const Book& rhs) const {
        return lhs.title < rhs.title;
    }
};

BinarySearchTree<Book, BookTitleLess> library;
library.emplace("Dune", 1965);
```

## Iterator usage

Source: [`examples/iterator_usage.cpp`](../examples/iterator_usage.cpp)

```cpp
auto lower = tree.lower_bound(23);
auto upper = tree.upper_bound(25);
auto next = tree.erase(tree.find(25));
```

## Traversal usage

Source: [`examples/traversal_usage.cpp`](../examples/traversal_usage.cpp)

```cpp
tree.in_order_traversal([](const int value) {
    std::cout << value << ' ';
});
```

## Suggested compile commands

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic examples/basic_usage.cpp -Iinclude -o basic_usage
g++ -std=c++17 -Wall -Wextra -Wpedantic examples/custom_object.cpp -Iinclude -o custom_object
g++ -std=c++17 -Wall -Wextra -Wpedantic examples/iterator_usage.cpp -Iinclude -o iterator_usage
g++ -std=c++17 -Wall -Wextra -Wpedantic examples/traversal_usage.cpp -Iinclude -o traversal_usage
```
