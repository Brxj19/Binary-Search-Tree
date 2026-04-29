# Performance

`BinarySearchTree` is a plain Binary Search Tree, so performance depends on tree height.

## Complexity table

| Operation | Average | Worst |
| --- | --- | --- |
| Search | `O(log N)` | `O(N)` |
| Insert | `O(log N)` | `O(N)` |
| Erase | `O(log N)` | `O(N)` |
| `lower_bound` / `upper_bound` | `O(log N)` | `O(N)` |
| Traversal | `O(N)` | `O(N)` |
| `size`, `empty` | `O(1)` | `O(1)` |

## Why the worst case matters

If values are inserted in already sorted order, the tree can become chain-like:

```text
1
 \
  2
   \
    3
     \
      4
```

That shape behaves more like a linked list than a balanced search tree.

## Practical guidance

- Use this library when you want a straightforward educational or lightweight BST.
- If guaranteed logarithmic performance is required, use a self-balancing tree or a standard container such as `std::set`.
- `height()` is useful for observing whether insertion order is making the tree tall and unbalanced.
