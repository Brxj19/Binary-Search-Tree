# API Reference

## `BinarySearchTree`

### Prototype

```cpp
template <typename T, typename Compare = std::less<T>>
class BinarySearchTree;
```

### Description

Stores unique values in a plain Binary Search Tree ordered by `Compare`.

### Notes

- Values are unique.
- Ordering is defined by the comparator, not necessarily by `operator==`.
- This is not a self-balancing tree.

## `BinarySearchTree(const Compare& compare = Compare())`

### Prototype

```cpp
explicit BinarySearchTree(const Compare& compare = Compare());
```

### Description

Constructs an empty tree.

### Parameters

- `compare`: comparator used to order inserted values.

### Return value

None.

### Complexity

Constant.

## `BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare())`

### Prototype

```cpp
template <typename InputIt>
BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare());
```

### Description

Constructs a tree by inserting every value from an iterator range.

### Parameters

- `first`: iterator to the first value.
- `last`: iterator one past the last value.
- `compare`: comparator used to order inserted values.

### Return value

None.

### Complexity

Average `O(N log N)`, worst `O(N^2)`.

## `BinarySearchTree(std::initializer_list<T> init, const Compare& compare = Compare())`

### Prototype

```cpp
BinarySearchTree(std::initializer_list<T> init, const Compare& compare = Compare());
```

### Description

Constructs a tree from an initializer list.

### Parameters

- `init`: values to insert.
- `compare`: comparator used to order inserted values.

### Return value

None.

### Complexity

Average `O(N log N)`, worst `O(N^2)`.

## `BinarySearchTree(const BinarySearchTree& other)`

### Prototype

```cpp
BinarySearchTree(const BinarySearchTree& other);
```

### Description

Creates a deep copy of another tree.

### Parameters

- `other`: source tree.

### Return value

None.

### Complexity

Linear in `other.size()`.

## `BinarySearchTree(BinarySearchTree&& other)`

### Prototype

```cpp
BinarySearchTree(BinarySearchTree&& other) noexcept;
```

### Description

Moves the contents of another tree into this one.

### Parameters

- `other`: source tree.

### Return value

None.

### Complexity

Constant.

## `operator=(const BinarySearchTree& other)`

### Prototype

```cpp
BinarySearchTree& operator=(const BinarySearchTree& other);
```

### Description

Replaces the current contents with a deep copy of another tree.

### Parameters

- `other`: source tree.

### Return value

- Reference to the assigned tree.

### Complexity

Linear in `other.size()`.

## `operator=(BinarySearchTree&& other)`

### Prototype

```cpp
BinarySearchTree& operator=(BinarySearchTree&& other) noexcept;
```

### Description

Moves another tree into this one.

### Parameters

- `other`: source tree.

### Return value

- Reference to the assigned tree.

### Complexity

Constant.

## `swap`

### Prototype

```cpp
void swap(BinarySearchTree& other);
```

### Description

Exchanges the contents of two trees.

### Parameters

- `other`: tree to swap with.

### Return value

None.

### Complexity

Constant.

## `size`

### Prototype

```cpp
size_type size() const noexcept;
```

### Description

Returns the number of stored elements.

### Parameters

None.

### Return value

- Current element count.

### Complexity

Constant.

## `empty`

### Prototype

```cpp
bool empty() const noexcept;
```

### Description

Checks whether the tree contains no elements.

### Parameters

None.

### Return value

- `true` if the tree is empty, `false` otherwise.

### Complexity

Constant.

## `clear`

### Prototype

```cpp
void clear() noexcept;
```

### Description

Removes all elements from the tree.

### Parameters

None.

### Return value

None.

### Complexity

Linear in `size()`.

## `insert(const T& value)`

### Prototype

```cpp
std::pair<iterator, bool> insert(const T& value);
```

### Description

Inserts a value by copying it if no equivalent value already exists.

### Parameters

- `value`: value to insert.

### Return value

- `iterator` points to the inserted or existing value.
- `bool` is `true` only when insertion actually happened.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `insert(T&& value)`

### Prototype

```cpp
std::pair<iterator, bool> insert(T&& value);
```

### Description

Inserts a value by moving it if no equivalent value already exists.

### Parameters

- `value`: value to insert.

### Return value

- `iterator` points to the inserted or existing value.
- `bool` is `true` only when insertion actually happened.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `insert(InputIt first, InputIt last)`

### Prototype

```cpp
template <typename InputIt>
void insert(InputIt first, InputIt last);
```

### Description

Inserts every value from a range.

### Parameters

- `first`: iterator to the first value.
- `last`: iterator one past the last value.

### Return value

None.

### Complexity

Average `O(M log(N + M))`, worst `O(M(N + M))`.

## `emplace`

### Prototype

```cpp
template <typename... Args>
std::pair<iterator, bool> emplace(Args&&... args);
```

### Description

Constructs a value of type `T` and inserts it if no equivalent value already exists.

### Parameters

- `args`: constructor arguments forwarded to `T`.

### Return value

- `iterator` points to the inserted or existing value.
- `bool` is `true` only when insertion actually happened.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `erase(const T& value)`

### Prototype

```cpp
size_type erase(const T& value);
```

### Description

Removes the value equivalent to `value`.

### Parameters

- `value`: value to remove.

### Return value

- `1` if an element was removed, `0` otherwise.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Notes

- Works for leaf deletion, one-child deletion, two-child deletion, root deletion, and empty-tree deletion attempts.

## `erase(const_iterator position)`

### Prototype

```cpp
iterator erase(const_iterator position);
```

### Description

Erases the element at `position`.

### Parameters

- `position`: iterator to the element to erase.

### Return value

- Iterator to the next element, or `end()` if none exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Notes

- Passing `end()` returns `end()`.

## `find`

### Prototype

```cpp
iterator find(const T& value);
const_iterator find(const T& value) const;
```

### Description

Searches for a value equivalent to `value`.

### Parameters

- `value`: lookup value.

### Return value

- Iterator to the matching element, or `end()` / `cend()` if not found.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `contains`

### Prototype

```cpp
bool contains(const T& value) const;
```

### Description

Checks whether a value exists in the tree.

### Parameters

- `value`: lookup value.

### Return value

- `true` if the value exists, `false` otherwise.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `lower_bound`

### Prototype

```cpp
iterator lower_bound(const T& value);
const_iterator lower_bound(const T& value) const;
```

### Description

Returns the first element that is not ordered before `value` according to `Compare`.

### Parameters

- `value`: lookup value.

### Return value

- Iterator to the lower bound, or `end()` / `cend()` if none exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `upper_bound`

### Prototype

```cpp
iterator upper_bound(const T& value);
const_iterator upper_bound(const T& value) const;
```

### Description

Returns the first element ordered after `value` according to `Compare`.

### Parameters

- `value`: lookup value.

### Return value

- Iterator to the upper bound, or `end()` / `cend()` if none exists.

### Complexity

Average `O(log N)`, worst `O(N)`.

## `begin`, `end`, `cbegin`, `cend`

### Prototype

```cpp
iterator begin() noexcept;
const_iterator begin() const noexcept;
const_iterator cbegin() const noexcept;
iterator end() noexcept;
const_iterator end() const noexcept;
const_iterator cend() const noexcept;
```

### Description

Provides bidirectional iterators for in-order traversal.

### Parameters

None.

### Return value

- `begin` and `cbegin` refer to the smallest element.
- `end` and `cend` refer to the past-the-end position.

### Complexity

- `begin` and `cbegin`: average `O(log N)`, worst `O(N)`.
- `end` and `cend`: constant.

## `min`

### Prototype

```cpp
T& min();
const T& min() const;
```

### Description

Returns the smallest value currently stored in the tree.

### Parameters

None.

### Return value

- Reference to the smallest element.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Notes

- Throws `std::out_of_range` if the tree is empty.

## `max`

### Prototype

```cpp
T& max();
const T& max() const;
```

### Description

Returns the largest value currently stored in the tree.

### Parameters

None.

### Return value

- Reference to the largest element.

### Complexity

Average `O(log N)`, worst `O(N)`.

### Notes

- Throws `std::out_of_range` if the tree is empty.

## `height`

### Prototype

```cpp
size_type height() const noexcept;
```

### Description

Returns the number of nodes on the longest root-to-leaf path.

### Parameters

None.

### Return value

- Tree height. An empty tree has height `0`.

### Complexity

Linear in `size()`.

## `to_vector`

### Prototype

```cpp
std::vector<T> to_vector() const;
```

### Description

Copies the tree contents into a vector using in-order traversal.

### Parameters

None.

### Return value

- A vector containing all values in comparator order.

### Complexity

Linear in `size()`.

## `in_order_traversal`

### Prototype

```cpp
template <typename UnaryFunction>
void in_order_traversal(UnaryFunction&& function) const;
```

### Description

Visits all nodes in sorted order.

### Parameters

- `function`: callable that accepts `const T&`.

### Return value

None.

### Complexity

Linear in `size()`.

## `pre_order_traversal`

### Prototype

```cpp
template <typename UnaryFunction>
void pre_order_traversal(UnaryFunction&& function) const;
```

### Description

Visits nodes in Root-Left-Right order.

### Parameters

- `function`: callable that accepts `const T&`.

### Return value

None.

### Complexity

Linear in `size()`.

## `post_order_traversal`

### Prototype

```cpp
template <typename UnaryFunction>
void post_order_traversal(UnaryFunction&& function) const;
```

### Description

Visits nodes in Left-Right-Root order.

### Parameters

- `function`: callable that accepts `const T&`.

### Return value

None.

### Complexity

Linear in `size()`.

## `is_valid_bst`

### Prototype

```cpp
bool is_valid_bst() const;
```

### Description

Checks whether the internal node relationships still satisfy the BST ordering rules.

### Parameters

None.

### Return value

- `true` if the tree is valid, `false` otherwise.

### Complexity

Linear in `size()`.
