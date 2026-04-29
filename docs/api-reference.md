# API Reference

This reference documents the public API of `BinarySearchTree<T, Compare>` from [`include/bst/bst.h`](../include/bst/bst.h).

## `BinarySearchTree`

### Prototype

```cpp
template <typename T, typename Compare = std::less<T>>
class BinarySearchTree;
```

### Description

`BinarySearchTree` stores unique values in comparator order using a plain Binary Search Tree with `std::unique_ptr`-owned nodes and bidirectional iterators.

### Parameters

- `T`: stored value type.
- `Compare`: comparator used to define ordering.

### Return value

Not applicable.

### Complexity

The empty object itself is constant-size. Operation complexity depends on the individual member function.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> numbers;
numbers.insert(8);
numbers.insert(3);
numbers.insert(10);
```

### Notes

- Values are unique.
- Ordering is defined by `Compare`.
- This is not a self-balancing tree.

### See also

- `insert(const T& value)`
- `find(const T& value)`
- `is_valid_bst()`

## `BinarySearchTree(const Compare& compare = Compare())`

### Prototype

```cpp
explicit BinarySearchTree(const Compare& compare = Compare());
```

### Description

Constructs an empty tree using the provided comparator.

### Parameters

- `compare`: comparator object used to order stored values.

### Return value

None.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> ascending;
BinarySearchTree<int, std::greater<int>> descending;

descending.insert(10);
descending.insert(20);
```

### Notes

- If no comparator is provided, `std::less<T>` is used.

### See also

- `BinarySearchTree(std::initializer_list<T> init, const Compare& compare = Compare())`
- `empty() const noexcept`

## `BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare())`

### Prototype

```cpp
template <typename InputIt>
BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare());
```

### Description

Constructs a tree by inserting all values in the range `[first, last)`.

### Parameters

- `first`: iterator to the first element in the input range.
- `last`: iterator one past the last element in the input range.
- `compare`: comparator object used to order stored values.

### Return value

None.

### Complexity

Average `O(N log N)`, worst `O(N^2)` for `N` inserted values.

### Complete small example

```cpp
#include <vector>
#include <bst/bst.h>

std::vector<int> values = {8, 3, 10, 1, 6};
BinarySearchTree<int> tree(values.begin(), values.end());
```

### Notes

- Duplicate values in the input range are ignored.

### See also

- `insert(InputIt first, InputIt last)`
- `to_vector() const`

## `BinarySearchTree(std::initializer_list<T> init, const Compare& compare = Compare())`

### Prototype

```cpp
BinarySearchTree(std::initializer_list<T> init, const Compare& compare = Compare());
```

### Description

Constructs a tree by inserting all values from an initializer list.

### Parameters

- `init`: initializer list containing values to insert.
- `compare`: comparator object used to order stored values.

### Return value

None.

### Complexity

Average `O(N log N)`, worst `O(N^2)` for `N` inserted values.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10, 1, 6};
```

### Notes

- Duplicate values are ignored.

### See also

- `insert(const T& value)`
- `BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare())`

## `BinarySearchTree(const BinarySearchTree& other)`

### Prototype

```cpp
BinarySearchTree(const BinarySearchTree& other);
```

### Description

Copy-constructs a deep copy of another tree.

### Parameters

- `other`: source tree to copy.

### Return value

None.

### Complexity

Linear in `other.size()`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> original = {8, 3, 10};
BinarySearchTree<int> copy(original);
```

### Notes

- The new tree owns its own nodes.
- Mutating the copy does not modify the original.

### See also

- `BinarySearchTree& operator=(const BinarySearchTree& other)`
- `swap(BinarySearchTree& other)`

## `BinarySearchTree(BinarySearchTree&& other) noexcept`

### Prototype

```cpp
BinarySearchTree(BinarySearchTree&& other) noexcept;
```

### Description

Move-constructs a tree by taking ownership of another tree's nodes.

### Parameters

- `other`: source tree to move from.

### Return value

None.

### Complexity

Constant.

### Complete small example

```cpp
#include <utility>
#include <bst/bst.h>

BinarySearchTree<int> source = {8, 3, 10};
BinarySearchTree<int> moved(std::move(source));
```

### Notes

- The moved-from tree is left empty.

### See also

- `BinarySearchTree& operator=(BinarySearchTree&& other) noexcept`
- `clear() noexcept`

## `~BinarySearchTree()`

### Prototype

```cpp
~BinarySearchTree() = default;
```

### Description

Destroys the tree and releases all owned nodes.

### Parameters

None.

### Return value

None.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
{
    BinarySearchTree<int> tree = {8, 3, 10};
} // tree is destroyed here
```

### Notes

- Cleanup is automatic because nodes are owned by `std::unique_ptr`.

### See also

- `clear() noexcept`

## `operator=(const BinarySearchTree& other)`

### Prototype

```cpp
BinarySearchTree& operator=(const BinarySearchTree& other);
```

### Description

Replaces the current contents with a deep copy of another tree.

### Parameters

- `other`: source tree to copy from.

### Return value

- Reference to the assigned tree.

### Complexity

Linear in `other.size()`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> source = {8, 3, 10};
BinarySearchTree<int> target;
target = source;
```

### Notes

- Self-assignment is handled safely.

### See also

- `BinarySearchTree(const BinarySearchTree& other)`
- `swap(BinarySearchTree& other)`

## `operator=(BinarySearchTree&& other) noexcept`

### Prototype

```cpp
BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;
```

### Description

Replaces the current contents by moving from another tree.

### Parameters

- `other`: source tree to move from.

### Return value

- Reference to the assigned tree.

### Complexity

Constant.

### Complete small example

```cpp
#include <utility>
#include <bst/bst.h>

BinarySearchTree<int> source = {8, 3, 10};
BinarySearchTree<int> target;
target = std::move(source);
```

### Notes

- The moved-from tree is left empty.

### See also

- `BinarySearchTree(BinarySearchTree&& other) noexcept`
- `clear() noexcept`

## `swap(BinarySearchTree& other)`

### Prototype

```cpp
void swap(BinarySearchTree& other);
```

### Description

Swaps the contents and comparator state of two trees.

### Parameters

- `other`: tree to exchange contents with.

### Return value

None.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> a = {1, 2};
BinarySearchTree<int> b = {8, 9};

a.swap(b);
```

### Notes

- Node ownership is exchanged without copying elements.

### See also

- `operator=(const BinarySearchTree& other)`
- `operator=(BinarySearchTree&& other) noexcept`

## `size() const noexcept`

### Prototype

```cpp
size_type size() const noexcept;
```

### Description

Returns the number of stored values.

### Parameters

None.

### Return value

- The current element count.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
auto count = tree.size();
```

### Notes

- Duplicate insertions do not increase the count.

### See also

- `empty() const noexcept`
- `insert(const T& value)`

## `empty() const noexcept`

### Prototype

```cpp
bool empty() const noexcept;
```

### Description

Checks whether the tree contains any values.

### Parameters

None.

### Return value

- `true` if the tree has no elements.
- `false` otherwise.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree;
bool has_values = !tree.empty();
```

### Notes

- This does not inspect individual nodes. It uses the stored size counter.

### See also

- `size() const noexcept`
- `clear() noexcept`

## `clear() noexcept`

### Prototype

```cpp
void clear() noexcept;
```

### Description

Removes all values from the tree.

### Parameters

None.

### Return value

None.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
tree.clear();
```

### Notes

- After `clear()`, `begin() == end()` and `size() == 0`.

### See also

- `empty() const noexcept`
- `erase(const T& value)`

## `insert(const T& value)`

### Prototype

```cpp
std::pair<iterator, bool> insert(const T& value);
```

### Description

Inserts a value by copy if no equivalent value already exists.

### Parameters

- `value`: value to insert.

### Return value

- `first`: iterator to the inserted element or the existing equivalent element.
- `second`: `true` if insertion happened, `false` if the value already existed.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree;
auto [it, inserted] = tree.insert(8);
```

### Notes

- Duplicate values are ignored.

### See also

- `insert(T&& value)`
- `emplace(Args&&... args)`

## `insert(T&& value)`

### Prototype

```cpp
std::pair<iterator, bool> insert(T&& value);
```

### Description

Inserts a value by move if no equivalent value already exists.

### Parameters

- `value`: value to move into the tree.

### Return value

- `first`: iterator to the inserted element or the existing equivalent element.
- `second`: `true` if insertion happened, `false` if the value already existed.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <string>
#include <utility>
#include <bst/bst.h>

BinarySearchTree<std::string> tree;
std::string value = "node";
auto [it, inserted] = tree.insert(std::move(value));
```

### Notes

- After insertion, the moved-from source object remains valid but unspecified.

### See also

- `insert(const T& value)`
- `emplace(Args&&... args)`

## `insert(InputIt first, InputIt last)`

### Prototype

```cpp
template <typename InputIt>
void insert(InputIt first, InputIt last);
```

### Description

Inserts all values in the range `[first, last)`.

### Parameters

- `first`: iterator to the first element in the range.
- `last`: iterator one past the last element in the range.

### Return value

None.

### Complexity

Average `O(M log(N + M))`, worst `O(M(N + M))` for `M` inserted values.

### Complete small example

```cpp
#include <vector>
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3};
std::vector<int> extra = {10, 1, 6};
tree.insert(extra.begin(), extra.end());
```

### Notes

- Equivalent values already in the tree are ignored.

### See also

- `BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare())`
- `insert(const T& value)`

## `emplace(Args&&... args)`

### Prototype

```cpp
template <typename... Args>
std::pair<iterator, bool> emplace(Args&&... args);
```

### Description

Constructs a `T` from the forwarded arguments and inserts it if no equivalent value exists.

### Parameters

- `args`: arguments forwarded to the constructor of `T`.

### Return value

- `first`: iterator to the inserted element or the existing equivalent element.
- `second`: `true` if insertion happened, `false` otherwise.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <string>
#include <bst/bst.h>

struct Record {
    int id;
    std::string name;
    Record(int i, std::string n) : id(i), name(std::move(n)) {}
    bool operator<(const Record& other) const { return id < other.id; }
};

BinarySearchTree<Record> tree;
auto [it, inserted] = tree.emplace(7, "seven");
```

### Notes

- This is often the most convenient insertion API for custom types.

### See also

- `insert(const T& value)`
- `insert(T&& value)`

## `erase(const T& value)`

### Prototype

```cpp
size_type erase(const T& value);
```

### Description

Erases the value equivalent to `value`, if it exists.

### Parameters

- `value`: value to remove.

### Return value

- `1` if a value was removed.
- `0` if no equivalent value was found.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
auto removed = tree.erase(3);
```

### Notes

- This overload is the closest match to `std::set::erase(const key_type&)`.
- It handles root, leaf, one-child, and two-child deletion.

### See also

- `erase(const_iterator position)`
- `contains(const T& value) const`

## `erase(const_iterator position)`

### Prototype

```cpp
iterator erase(const_iterator position);
```

### Description

Erases the element referred to by `position`.

### Parameters

- `position`: iterator naming the element to erase.

### Return value

- Iterator to the element that follows the erased one.
- `end()` if the erased element was the last one or if `position == end()`.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
auto it = tree.find(8);
auto next = tree.erase(it);
```

### Notes

- Passing `end()` returns `end()`.
- The returned iterator never points to the deleted node.

### See also

- `find(const T& value)`
- `erase(const T& value)`

## `find(const T& value)`

### Prototype

```cpp
iterator find(const T& value);
```

### Description

Searches for a value equivalent to `value` and returns a mutable iterator if found.

### Parameters

- `value`: lookup value.

### Return value

- Iterator to the matching element.
- `end()` if no equivalent value exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
auto it = tree.find(3);
```

### Notes

- The returned iterator traverses the tree in comparator order.

### See also

- `find(const T& value) const`
- `contains(const T& value) const`

## `find(const T& value) const`

### Prototype

```cpp
const_iterator find(const T& value) const;
```

### Description

Searches for a value equivalent to `value` and returns a const iterator if found.

### Parameters

- `value`: lookup value.

### Return value

- Const iterator to the matching element.
- `cend()` if no equivalent value exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
auto it = tree.find(10);
```

### Notes

- Useful when iterating over a const tree.

### See also

- `cbegin() const noexcept`
- `contains(const T& value) const`

## `contains(const T& value) const`

### Prototype

```cpp
bool contains(const T& value) const;
```

### Description

Checks whether an equivalent value exists in the tree.

### Parameters

- `value`: lookup value.

### Return value

- `true` if a matching value exists.
- `false` otherwise.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
bool has_three = tree.contains(3);
```

### Notes

- This is a convenience wrapper around tree lookup.

### See also

- `find(const T& value)`
- `erase(const T& value)`

## `lower_bound(const T& value)`

### Prototype

```cpp
iterator lower_bound(const T& value);
```

### Description

Returns an iterator to the first element that is not ordered before `value`.

### Parameters

- `value`: lookup value.

### Return value

- Iterator to the lower bound.
- `end()` if no such element exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10, 6};
auto it = tree.lower_bound(5);
```

### Notes

- Comparator order determines what “not ordered before” means.

### See also

- `upper_bound(const T& value)`
- `find(const T& value)`

## `lower_bound(const T& value) const`

### Prototype

```cpp
const_iterator lower_bound(const T& value) const;
```

### Description

Returns a const iterator to the first element that is not ordered before `value`.

### Parameters

- `value`: lookup value.

### Return value

- Const iterator to the lower bound.
- `cend()` if no such element exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10, 6};
auto it = tree.lower_bound(5);
```

### Notes

- Use this overload when the tree itself is const.

### See also

- `lower_bound(const T& value)`
- `cbegin() const noexcept`

## `upper_bound(const T& value)`

### Prototype

```cpp
iterator upper_bound(const T& value);
```

### Description

Returns an iterator to the first element ordered after `value`.

### Parameters

- `value`: lookup value.

### Return value

- Iterator to the upper bound.
- `end()` if no such element exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10, 6};
auto it = tree.upper_bound(6);
```

### Notes

- This is useful for stepping to the next strictly greater element.

### See also

- `lower_bound(const T& value)`
- `find(const T& value)`

## `upper_bound(const T& value) const`

### Prototype

```cpp
const_iterator upper_bound(const T& value) const;
```

### Description

Returns a const iterator to the first element ordered after `value`.

### Parameters

- `value`: lookup value.

### Return value

- Const iterator to the upper bound.
- `cend()` if no such element exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10, 6};
auto it = tree.upper_bound(6);
```

### Notes

- Use this overload when the tree itself is const.

### See also

- `upper_bound(const T& value)`
- `cend() const noexcept`

## `begin() noexcept`

### Prototype

```cpp
iterator begin() noexcept;
```

### Description

Returns an iterator to the smallest element in comparator order.

### Parameters

None.

### Return value

- Iterator to the first element.
- `end()` if the tree is empty.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
auto it = tree.begin();
```

### Notes

- Iteration follows in-order traversal.

### See also

- `end() noexcept`
- `min()`

## `begin() const noexcept`

### Prototype

```cpp
const_iterator begin() const noexcept;
```

### Description

Returns a const iterator to the smallest element in comparator order.

### Parameters

None.

### Return value

- Const iterator to the first element.
- `cend()` if the tree is empty.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
auto it = tree.begin();
```

### Notes

- This overload is selected automatically for const trees.

### See also

- `cbegin() const noexcept`
- `find(const T& value) const`

## `cbegin() const noexcept`

### Prototype

```cpp
const_iterator cbegin() const noexcept;
```

### Description

Returns a const iterator to the first element.

### Parameters

None.

### Return value

- Const iterator to the first element.
- `cend()` if the tree is empty.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
auto it = tree.cbegin();
```

### Notes

- This is equivalent to `begin()` for a const tree.

### See also

- `begin() const noexcept`
- `cend() const noexcept`

## `end() noexcept`

### Prototype

```cpp
iterator end() noexcept;
```

### Description

Returns the past-the-end iterator.

### Parameters

None.

### Return value

- Iterator representing one past the last element.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
for (auto it = tree.begin(); it != tree.end(); ++it) {
    (void)*it;
}
```

### Notes

- `--end()` is valid for non-empty trees and yields the largest element.

### See also

- `begin() noexcept`
- `max()`

## `end() const noexcept`

### Prototype

```cpp
const_iterator end() const noexcept;
```

### Description

Returns the const past-the-end iterator.

### Parameters

None.

### Return value

- Const iterator representing one past the last element.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
for (auto it = tree.begin(); it != tree.end(); ++it) {
    (void)*it;
}
```

### Notes

- This overload is selected for const trees.

### See also

- `cend() const noexcept`
- `begin() const noexcept`

## `cend() const noexcept`

### Prototype

```cpp
const_iterator cend() const noexcept;
```

### Description

Returns the const past-the-end iterator.

### Parameters

None.

### Return value

- Const iterator representing one past the last element.

### Complexity

Constant.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
bool empty_range = (tree.cbegin() == tree.cend());
```

### Notes

- This is equivalent to `end()` for a const tree.

### See also

- `cbegin() const noexcept`
- `end() const noexcept`

## `min()`

### Prototype

```cpp
T& min();
```

### Description

Returns a mutable reference to the smallest stored value.

### Parameters

None.

### Return value

- Reference to the smallest element.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
int smallest = tree.min();
```

### Notes

- Throws `std::out_of_range` if the tree is empty.

### See also

- `const T& min() const`
- `begin() noexcept`

## `min() const`

### Prototype

```cpp
const T& min() const;
```

### Description

Returns a const reference to the smallest stored value.

### Parameters

None.

### Return value

- Const reference to the smallest element.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
int smallest = tree.min();
```

### Notes

- Throws `std::out_of_range` if the tree is empty.

### See also

- `T& min()`
- `const_iterator begin() const noexcept`

## `max()`

### Prototype

```cpp
T& max();
```

### Description

Returns a mutable reference to the largest stored value.

### Parameters

None.

### Return value

- Reference to the largest element.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
int largest = tree.max();
```

### Notes

- Throws `std::out_of_range` if the tree is empty.

### See also

- `const T& max() const`
- `end() noexcept`

## `max() const`

### Prototype

```cpp
const T& max() const;
```

### Description

Returns a const reference to the largest stored value.

### Parameters

None.

### Return value

- Const reference to the largest element.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Complete small example

```cpp
#include <bst/bst.h>

const BinarySearchTree<int> tree = {8, 3, 10};
int largest = tree.max();
```

### Notes

- Throws `std::out_of_range` if the tree is empty.

### See also

- `T& max()`
- `const_iterator end() const noexcept`

## `height() const noexcept`

### Prototype

```cpp
size_type height() const noexcept;
```

### Description

Returns the number of nodes on the longest root-to-leaf path.

### Parameters

None.

### Return value

- Tree height.
- `0` for an empty tree.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10, 1, 6};
auto tree_height = tree.height();
```

### Notes

- Taller trees usually mean worse lookup, insertion, and erase costs.

### See also

- `is_valid_bst() const`
- `to_vector() const`

## `to_vector() const`

### Prototype

```cpp
std::vector<T> to_vector() const;
```

### Description

Returns all stored values in comparator order inside a `std::vector`.

### Parameters

None.

### Return value

- Vector containing the tree contents in sorted comparator order.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <vector>
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
std::vector<int> values = tree.to_vector();
```

### Notes

- The output order matches in-order traversal.

### See also

- `in_order_traversal(UnaryFunction&& function) const`
- `begin() noexcept`

## `in_order_traversal(UnaryFunction&& function) const`

### Prototype

```cpp
template <typename UnaryFunction>
void in_order_traversal(UnaryFunction&& function) const;
```

### Description

Visits all values in comparator order.

### Parameters

- `function`: callable receiving each visited value as `const T&`.

### Return value

None.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <iostream>
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
tree.in_order_traversal([](const int value) {
    std::cout << value << ' ';
});
```

### Notes

- In-order traversal is the sorted traversal of the tree.

### See also

- `pre_order_traversal(UnaryFunction&& function) const`
- `post_order_traversal(UnaryFunction&& function) const`

## `pre_order_traversal(UnaryFunction&& function) const`

### Prototype

```cpp
template <typename UnaryFunction>
void pre_order_traversal(UnaryFunction&& function) const;
```

### Description

Visits values in Root-Left-Right order.

### Parameters

- `function`: callable receiving each visited value as `const T&`.

### Return value

None.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <iostream>
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
tree.pre_order_traversal([](const int value) {
    std::cout << value << ' ';
});
```

### Notes

- Pre-order traversal is often useful for copying or serialization-style tasks.

### See also

- `in_order_traversal(UnaryFunction&& function) const`
- `post_order_traversal(UnaryFunction&& function) const`

## `post_order_traversal(UnaryFunction&& function) const`

### Prototype

```cpp
template <typename UnaryFunction>
void post_order_traversal(UnaryFunction&& function) const;
```

### Description

Visits values in Left-Right-Root order.

### Parameters

- `function`: callable receiving each visited value as `const T&`.

### Return value

None.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <iostream>
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
tree.post_order_traversal([](const int value) {
    std::cout << value << ' ';
});
```

### Notes

- Post-order traversal is often useful when children must be processed before parents.

### See also

- `in_order_traversal(UnaryFunction&& function) const`
- `pre_order_traversal(UnaryFunction&& function) const`

## `is_valid_bst() const`

### Prototype

```cpp
bool is_valid_bst() const;
```

### Description

Checks whether the internal node ordering still satisfies the Binary Search Tree property under the configured comparator.

### Parameters

None.

### Return value

- `true` if the tree ordering is valid.
- `false` otherwise.

### Complexity

Linear in `size()`.

### Complete small example

```cpp
#include <bst/bst.h>

BinarySearchTree<int> tree = {8, 3, 10};
bool valid = tree.is_valid_bst();
```

### Notes

- This is mainly a diagnostic helper for tests, debugging, and learning.

### See also

- `height() const noexcept`
- `to_vector() const`
