#ifndef BST_BST_H
#define BST_BST_H

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

/**
 * @brief A header-only Binary Search Tree container.
 *
 * `BinarySearchTree` stores unique values in sorted order using a classic
 * binary search tree. The tree uses `std::unique_ptr` internally for
 * ownership, supports bidirectional iterators, and provides STL-style lookup
 * and insertion APIs.
 *
 * This container is intentionally a plain Binary Search Tree. It does not
 * rebalance itself, so operation costs depend on tree shape.
 *
 * @tparam T Stored value type.
 * @tparam Compare Strict weak ordering used to compare values.
 *
 * @complexity
 * Construction of an empty tree is O(1).
 *
 * @note
 * This container does not allow duplicate values.
 */
template <typename T, typename Compare = std::less<T>>
class BinarySearchTree {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using value_compare = Compare;
    using reference = value_type&;
    using const_reference = const value_type&;

private:
    struct Node {
        value_type value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node* parent;

        explicit Node(const value_type& new_value, Node* new_parent = nullptr)
            : value(new_value), left(nullptr), right(nullptr), parent(new_parent) {}

        explicit Node(value_type&& new_value, Node* new_parent = nullptr)
            : value(std::move(new_value)), left(nullptr), right(nullptr), parent(new_parent) {}
    };

    using node_ptr = std::unique_ptr<Node>;

    node_ptr root_;
    size_type size_;
    Compare compare_;

    static bool equivalent(const value_type& lhs, const value_type& rhs, const Compare& compare) {
        return !compare(lhs, rhs) && !compare(rhs, lhs);
    }

    template <typename ValueType, typename Pointer, typename Reference>
    class tree_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = Pointer;
        using reference = Reference;

        tree_iterator() : node_(nullptr), tree_(nullptr) {}

        template <typename OtherValueType, typename OtherPointer, typename OtherReference,
                  typename = std::enable_if_t<std::is_convertible<OtherPointer, Pointer>::value>>
        tree_iterator(const tree_iterator<OtherValueType, OtherPointer, OtherReference>& other)
            : node_(other.node_), tree_(other.tree_) {}

        reference operator*() const {
            return node_->value;
        }

        pointer operator->() const {
            return std::addressof(node_->value);
        }

        tree_iterator& operator++() {
            if (tree_ != nullptr) {
                node_ = tree_->successor(node_);
            }
            return *this;
        }

        tree_iterator operator++(int) {
            tree_iterator copy(*this);
            ++(*this);
            return copy;
        }

        tree_iterator& operator--() {
            if (tree_ == nullptr) {
                return *this;
            }

            if (node_ == nullptr) {
                node_ = tree_->max_node(tree_->root_.get());
            } else {
                node_ = tree_->predecessor(node_);
            }
            return *this;
        }

        tree_iterator operator--(int) {
            tree_iterator copy(*this);
            --(*this);
            return copy;
        }

        template <typename OtherValueType, typename OtherPointer, typename OtherReference>
        bool operator==(const tree_iterator<OtherValueType, OtherPointer, OtherReference>& other) const {
            return node_ == other.node_ && tree_ == other.tree_;
        }

        template <typename OtherValueType, typename OtherPointer, typename OtherReference>
        bool operator!=(const tree_iterator<OtherValueType, OtherPointer, OtherReference>& other) const {
            return !(*this == other);
        }

    private:
        Node* node_;
        const BinarySearchTree* tree_;

        explicit tree_iterator(Node* node, const BinarySearchTree* tree) : node_(node), tree_(tree) {}

        template <typename, typename, typename>
        friend class tree_iterator;
        friend class BinarySearchTree;
    };

public:
    using iterator = tree_iterator<value_type, value_type*, value_type&>;
    using const_iterator = tree_iterator<const value_type, const value_type*, const value_type&>;

    /**
     * @brief Constructs an empty binary search tree.
     *
     * @param compare Comparison object used to order elements.
     *
     * @complexity
     * Constant.
     */
    explicit BinarySearchTree(const Compare& compare = Compare())
        : root_(nullptr), size_(0), compare_(compare) {}

    /**
     * @brief Constructs a tree from an iterator range.
     *
     * Existing duplicate values are ignored.
     *
     * @tparam InputIt Input iterator type.
     * @param first Iterator to the first element in the range.
     * @param last Iterator one past the last element in the range.
     * @param compare Comparison object used to order elements.
     *
     * @complexity
     * Average: O(N log N) for N inserted elements when the tree stays reasonably balanced.
     * Worst: O(N^2) when the tree becomes highly unbalanced.
     */
    template <typename InputIt>
    BinarySearchTree(InputIt first, InputIt last, const Compare& compare = Compare())
        : BinarySearchTree(compare) {
        insert(first, last);
    }

    /**
     * @brief Constructs a tree from an initializer list.
     *
     * Duplicate values are ignored.
     *
     * @param init Initial values to insert.
     * @param compare Comparison object used to order elements.
     *
     * @complexity
     * Average: O(N log N) for N inserted elements when the tree stays reasonably balanced.
     * Worst: O(N^2) when the tree becomes highly unbalanced.
     */
    BinarySearchTree(std::initializer_list<T> init, const Compare& compare = Compare())
        : BinarySearchTree(compare) {
        insert(init.begin(), init.end());
    }

    /**
     * @brief Copy-constructs a tree from another tree.
     *
     * @param other Tree to copy.
     *
     * @complexity
     * Linear in `other.size()`.
     */
    BinarySearchTree(const BinarySearchTree& other)
        : root_(clone_subtree(other.root_.get(), nullptr)),
          size_(other.size_),
          compare_(other.compare_) {}

    /**
     * @brief Move-constructs a tree from another tree.
     *
     * @param other Tree to move from.
     *
     * @complexity
     * Constant.
     */
    BinarySearchTree(BinarySearchTree&& other) noexcept
        : root_(std::move(other.root_)),
          size_(other.size_),
          compare_(std::move(other.compare_)) {
        other.size_ = 0;
    }

    /**
     * @brief Destroys the tree.
     *
     * @complexity
     * Linear in `size()`.
     */
    ~BinarySearchTree() = default;

    /**
     * @brief Copy-assigns the contents of another tree.
     *
     * @param other Tree to copy from.
     * @return BinarySearchTree& Reference to this tree.
     *
     * @complexity
     * Linear in `other.size()`.
     */
    BinarySearchTree& operator=(const BinarySearchTree& other) {
        if (this != &other) {
            BinarySearchTree copy(other);
            swap(copy);
        }
        return *this;
    }

    /**
     * @brief Move-assigns the contents of another tree.
     *
     * @param other Tree to move from.
     * @return BinarySearchTree& Reference to this tree.
     *
     * @complexity
     * Constant.
     */
    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            root_ = std::move(other.root_);
            size_ = other.size_;
            compare_ = std::move(other.compare_);
            other.size_ = 0;
        }
        return *this;
    }

    /**
     * @brief Swaps the contents of two trees.
     *
     * @param other Tree to swap with.
     *
     * @complexity
     * Constant.
     */
    void swap(BinarySearchTree& other) {
        using std::swap;
        swap(root_, other.root_);
        swap(size_, other.size_);
        swap(compare_, other.compare_);
    }

    /**
     * @brief Returns the number of stored elements.
     *
     * @return size_type Current element count.
     *
     * @complexity
     * Constant.
     */
    size_type size() const noexcept {
        return size_;
    }

    /**
     * @brief Checks whether the tree is empty.
     *
     * @return bool `true` when the tree has no elements, otherwise `false`.
     *
     * @complexity
     * Constant.
     */
    bool empty() const noexcept {
        return size_ == 0;
    }

    /**
     * @brief Removes all elements from the tree.
     *
     * @complexity
     * Linear in `size()`.
     */
    void clear() noexcept {
        root_.reset();
        size_ = 0;
    }

    /**
     * @brief Inserts a value into the tree by copying it.
     *
     * If the value already exists, no new node is inserted.
     *
     * @param value The value to insert.
     * @return std::pair<iterator, bool>
     *         - `iterator` points to the inserted or existing element.
     *         - `bool` is `true` if insertion happened, `false` otherwise.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    std::pair<iterator, bool> insert(const T& value) {
        return insert_impl(value);
    }

    /**
     * @brief Inserts a value into the tree by moving it.
     *
     * If the value already exists, no new node is inserted.
     *
     * @param value The value to insert.
     * @return std::pair<iterator, bool>
     *         - `iterator` points to the inserted or existing element.
     *         - `bool` is `true` if insertion happened, `false` otherwise.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    std::pair<iterator, bool> insert(T&& value) {
        return insert_impl(std::move(value));
    }

    /**
     * @brief Inserts each element in a range.
     *
     * Duplicate values are ignored.
     *
     * @tparam InputIt Input iterator type.
     * @param first Iterator to the first element to insert.
     * @param last Iterator one past the last element to insert.
     *
     * @complexity
     * Average: O(M log(N + M)) for M inserted elements when the tree stays reasonably balanced.
     * Worst: O(M(N + M)) when the tree is highly unbalanced.
     */
    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            insert(*first);
        }
    }

    /**
     * @brief Constructs a value in place and inserts it into the tree.
     *
     * If an equivalent value already exists, no new node is inserted.
     *
     * @tparam Args Constructor argument types for `T`.
     * @param args Arguments forwarded to `T`'s constructor.
     * @return std::pair<iterator, bool>
     *         - `iterator` points to the inserted or existing element.
     *         - `bool` is `true` if insertion happened, `false` otherwise.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        T value(std::forward<Args>(args)...);
        return insert_impl(std::move(value));
    }

    /**
     * @brief Erases a value from the tree.
     *
     * @param value The value to erase.
     * @return size_type `1` if an element was erased, otherwise `0`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     *
     * @note
     * This overload removes by key, similar to `std::set::erase(const key_type&)`.
     */
    size_type erase(const T& value) {
        node_ptr* link = find_link(value);
        if (link == nullptr || *link == nullptr) {
            return 0;
        }

        erase_node(link);
        return 1;
    }

    /**
     * @brief Erases the element at an iterator position.
     *
     * @param position Iterator pointing to the element to erase.
     * @return iterator Iterator to the element that follows the erased one, or `end()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     *
     * @note
     * Passing `end()` returns `end()`.
     */
    iterator erase(const_iterator position) {
        if (position.tree_ != this || position.node_ == nullptr) {
            return end();
        }

        Node* next = successor(position.node_);
        erase_node(link_from_node(position.node_));
        return iterator(next, this);
    }

    /**
     * @brief Finds an element equal to `value`.
     *
     * @param value The value to search for.
     * @return iterator Iterator to the element if found, otherwise `end()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    iterator find(const T& value) {
        return iterator(find_node(value), this);
    }

    /**
     * @brief Finds an element equal to `value`.
     *
     * @param value The value to search for.
     * @return const_iterator Iterator to the element if found, otherwise `cend()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    const_iterator find(const T& value) const {
        return const_iterator(find_node(value), this);
    }

    /**
     * @brief Checks whether a value exists in the tree.
     *
     * @param value The value to search for.
     * @return bool `true` if the value exists, otherwise `false`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    bool contains(const T& value) const {
        return find_node(value) != nullptr;
    }

    /**
     * @brief Returns an iterator to the first element not less than `value`.
     *
     * @param value Lookup value.
     * @return iterator Iterator to the lower bound, or `end()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    iterator lower_bound(const T& value) {
        return iterator(lower_bound_node(value), this);
    }

    /**
     * @brief Returns an iterator to the first element not less than `value`.
     *
     * @param value Lookup value.
     * @return const_iterator Iterator to the lower bound, or `cend()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    const_iterator lower_bound(const T& value) const {
        return const_iterator(lower_bound_node(value), this);
    }

    /**
     * @brief Returns an iterator to the first element greater than `value`.
     *
     * @param value Lookup value.
     * @return iterator Iterator to the upper bound, or `end()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    iterator upper_bound(const T& value) {
        return iterator(upper_bound_node(value), this);
    }

    /**
     * @brief Returns an iterator to the first element greater than `value`.
     *
     * @param value Lookup value.
     * @return const_iterator Iterator to the upper bound, or `cend()`.
     *
     * @complexity
     * Average: O(log N) when the tree is reasonably balanced.
     * Worst: O(N) when the tree is highly unbalanced.
     */
    const_iterator upper_bound(const T& value) const {
        return const_iterator(upper_bound_node(value), this);
    }

    /**
     * @brief Returns an iterator to the smallest element.
     *
     * @return iterator Iterator to the first element, or `end()` if the tree is empty.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     */
    iterator begin() noexcept {
        return iterator(min_node(root_.get()), this);
    }

    /**
     * @brief Returns an iterator to the smallest element.
     *
     * @return const_iterator Iterator to the first element, or `cend()` if the tree is empty.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     */
    const_iterator begin() const noexcept {
        return const_iterator(min_node(root_.get()), this);
    }

    /**
     * @brief Returns a const iterator to the smallest element.
     *
     * @return const_iterator Iterator to the first element, or `cend()` if the tree is empty.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     */
    const_iterator cbegin() const noexcept {
        return begin();
    }

    /**
     * @brief Returns an iterator representing one past the last element.
     *
     * @return iterator End iterator.
     *
     * @complexity
     * Constant.
     */
    iterator end() noexcept {
        return iterator(nullptr, this);
    }

    /**
     * @brief Returns a const iterator representing one past the last element.
     *
     * @return const_iterator End iterator.
     *
     * @complexity
     * Constant.
     */
    const_iterator end() const noexcept {
        return const_iterator(nullptr, this);
    }

    /**
     * @brief Returns a const iterator representing one past the last element.
     *
     * @return const_iterator End iterator.
     *
     * @complexity
     * Constant.
     */
    const_iterator cend() const noexcept {
        return end();
    }

    /**
     * @brief Returns the smallest value in the tree.
     *
     * @return T& Reference to the smallest stored value.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     *
     * @throws std::out_of_range If the tree is empty.
     */
    T& min() {
        Node* node = min_node(root_.get());
        if (node == nullptr) {
            throw std::out_of_range("BinarySearchTree::min() called on an empty tree");
        }
        return node->value;
    }

    /**
     * @brief Returns the smallest value in the tree.
     *
     * @return const T& Reference to the smallest stored value.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     *
     * @throws std::out_of_range If the tree is empty.
     */
    const T& min() const {
        const Node* node = min_node(root_.get());
        if (node == nullptr) {
            throw std::out_of_range("BinarySearchTree::min() called on an empty tree");
        }
        return node->value;
    }

    /**
     * @brief Returns the largest value in the tree.
     *
     * @return T& Reference to the largest stored value.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     *
     * @throws std::out_of_range If the tree is empty.
     */
    T& max() {
        Node* node = max_node(root_.get());
        if (node == nullptr) {
            throw std::out_of_range("BinarySearchTree::max() called on an empty tree");
        }
        return node->value;
    }

    /**
     * @brief Returns the largest value in the tree.
     *
     * @return const T& Reference to the largest stored value.
     *
     * @complexity
     * Average: O(log N).
     * Worst: O(N).
     *
     * @throws std::out_of_range If the tree is empty.
     */
    const T& max() const {
        const Node* node = max_node(root_.get());
        if (node == nullptr) {
            throw std::out_of_range("BinarySearchTree::max() called on an empty tree");
        }
        return node->value;
    }

    /**
     * @brief Returns the height of the tree.
     *
     * This implementation reports the number of nodes on the longest path from
     * the root to a leaf. An empty tree has height `0`.
     *
     * @return size_type Tree height.
     *
     * @complexity
     * Linear in `size()`.
     */
    size_type height() const noexcept {
        return height_of(root_.get());
    }

    /**
     * @brief Copies the tree contents into a sorted vector.
     *
     * @return std::vector<T> Values produced by in-order traversal.
     *
     * @complexity
     * Linear in `size()`.
     */
    std::vector<T> to_vector() const {
        std::vector<T> values;
        values.reserve(size_);
        in_order_traversal([&values](const T& value) {
            values.push_back(value);
        });
        return values;
    }

    /**
     * @brief Visits elements using in-order traversal.
     *
     * In-order traversal visits nodes in sorted order.
     *
     * @tparam UnaryFunction Callable type accepting `const T&`.
     * @param function Callable invoked for each visited element.
     *
     * @complexity
     * Linear in `size()`.
     */
    template <typename UnaryFunction>
    void in_order_traversal(UnaryFunction&& function) const {
        auto visitor = std::forward<UnaryFunction>(function);
        in_order_impl(root_.get(), visitor);
    }

    /**
     * @brief Visits elements using pre-order traversal.
     *
     * Pre-order traversal visits nodes in Root-Left-Right order.
     *
     * @tparam UnaryFunction Callable type accepting `const T&`.
     * @param function Callable invoked for each visited element.
     *
     * @complexity
     * Linear in `size()`.
     */
    template <typename UnaryFunction>
    void pre_order_traversal(UnaryFunction&& function) const {
        auto visitor = std::forward<UnaryFunction>(function);
        pre_order_impl(root_.get(), visitor);
    }

    /**
     * @brief Visits elements using post-order traversal.
     *
     * Post-order traversal visits nodes in Left-Right-Root order.
     *
     * @tparam UnaryFunction Callable type accepting `const T&`.
     * @param function Callable invoked for each visited element.
     *
     * @complexity
     * Linear in `size()`.
     */
    template <typename UnaryFunction>
    void post_order_traversal(UnaryFunction&& function) const {
        auto visitor = std::forward<UnaryFunction>(function);
        post_order_impl(root_.get(), visitor);
    }

    /**
     * @brief Verifies that the tree still satisfies Binary Search Tree ordering.
     *
     * @return bool `true` if the tree is valid, otherwise `false`.
     *
     * @complexity
     * Linear in `size()`.
     */
    bool is_valid_bst() const {
        return is_valid_subtree(root_.get(), nullptr, nullptr);
    }

private:
    template <typename Value>
    std::pair<iterator, bool> insert_impl(Value&& value) {
        node_ptr* current = &root_;
        Node* parent = nullptr;

        while (*current != nullptr) {
            parent = current->get();
            if (compare_(value, parent->value)) {
                current = &parent->left;
            } else if (compare_(parent->value, value)) {
                current = &parent->right;
            } else {
                return std::make_pair(iterator(parent, this), false);
            }
        }

        *current = std::make_unique<Node>(std::forward<Value>(value), parent);
        ++size_;
        return std::make_pair(iterator(current->get(), this), true);
    }

    Node* find_node(const T& value) const {
        Node* current = root_.get();

        while (current != nullptr) {
            if (compare_(value, current->value)) {
                current = current->left.get();
            } else if (compare_(current->value, value)) {
                current = current->right.get();
            } else {
                return current;
            }
        }

        return nullptr;
    }

    node_ptr* find_link(const T& value) {
        node_ptr* current = &root_;

        while (*current != nullptr) {
            if (compare_(value, (*current)->value)) {
                current = &(*current)->left;
            } else if (compare_((*current)->value, value)) {
                current = &(*current)->right;
            } else {
                return current;
            }
        }

        return current;
    }

    node_ptr* link_from_node(Node* node) {
        if (node->parent == nullptr) {
            return &root_;
        }

        if (node->parent->left.get() == node) {
            return &node->parent->left;
        }

        return &node->parent->right;
    }

    void erase_node(node_ptr* target_link) {
        node_ptr removed = std::move(*target_link);
        Node* parent = removed->parent;

        if (removed->left == nullptr) {
            *target_link = std::move(removed->right);
            if (*target_link != nullptr) {
                (*target_link)->parent = parent;
            }
        } else if (removed->right == nullptr) {
            *target_link = std::move(removed->left);
            if (*target_link != nullptr) {
                (*target_link)->parent = parent;
            }
        } else if (removed->right->left == nullptr) {
            node_ptr successor = std::move(removed->right);
            successor->left = std::move(removed->left);
            successor->left->parent = successor.get();
            successor->parent = parent;
            *target_link = std::move(successor);
        } else {
            node_ptr* successor_link = &removed->right;
            while ((*successor_link)->left != nullptr) {
                successor_link = &(*successor_link)->left;
            }

            node_ptr successor = std::move(*successor_link);
            Node* successor_parent = successor->parent;

            *successor_link = std::move(successor->right);
            if (*successor_link != nullptr) {
                (*successor_link)->parent = successor_parent;
            }

            successor->left = std::move(removed->left);
            successor->left->parent = successor.get();
            successor->right = std::move(removed->right);
            successor->right->parent = successor.get();
            successor->parent = parent;
            *target_link = std::move(successor);
        }

        --size_;
    }

    Node* lower_bound_node(const T& value) const {
        Node* current = root_.get();
        Node* candidate = nullptr;

        while (current != nullptr) {
            if (!compare_(current->value, value)) {
                candidate = current;
                current = current->left.get();
            } else {
                current = current->right.get();
            }
        }

        return candidate;
    }

    Node* upper_bound_node(const T& value) const {
        Node* current = root_.get();
        Node* candidate = nullptr;

        while (current != nullptr) {
            if (compare_(value, current->value)) {
                candidate = current;
                current = current->left.get();
            } else {
                current = current->right.get();
            }
        }

        return candidate;
    }

    static Node* min_node(Node* node) noexcept {
        while (node != nullptr && node->left != nullptr) {
            node = node->left.get();
        }
        return node;
    }

    static const Node* min_node(const Node* node) noexcept {
        while (node != nullptr && node->left != nullptr) {
            node = node->left.get();
        }
        return node;
    }

    static Node* max_node(Node* node) noexcept {
        while (node != nullptr && node->right != nullptr) {
            node = node->right.get();
        }
        return node;
    }

    static const Node* max_node(const Node* node) noexcept {
        while (node != nullptr && node->right != nullptr) {
            node = node->right.get();
        }
        return node;
    }

    Node* successor(Node* node) const noexcept {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->right != nullptr) {
            return min_node(node->right.get());
        }

        Node* parent = node->parent;
        while (parent != nullptr && parent->right.get() == node) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    Node* predecessor(Node* node) const noexcept {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->left != nullptr) {
            return max_node(node->left.get());
        }

        Node* parent = node->parent;
        while (parent != nullptr && parent->left.get() == node) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    static node_ptr clone_subtree(const Node* other, Node* parent) {
        if (other == nullptr) {
            return nullptr;
        }

        node_ptr copy = std::make_unique<Node>(other->value, parent);
        copy->left = clone_subtree(other->left.get(), copy.get());
        copy->right = clone_subtree(other->right.get(), copy.get());
        return copy;
    }

    static size_type height_of(const Node* node) noexcept {
        if (node == nullptr) {
            return 0;
        }

        const size_type left_height = height_of(node->left.get());
        const size_type right_height = height_of(node->right.get());
        return 1 + (left_height > right_height ? left_height : right_height);
    }

    template <typename UnaryFunction>
    static void in_order_impl(const Node* node, UnaryFunction& function) {
        if (node == nullptr) {
            return;
        }

        in_order_impl(node->left.get(), function);
        function(node->value);
        in_order_impl(node->right.get(), function);
    }

    template <typename UnaryFunction>
    static void pre_order_impl(const Node* node, UnaryFunction& function) {
        if (node == nullptr) {
            return;
        }

        function(node->value);
        pre_order_impl(node->left.get(), function);
        pre_order_impl(node->right.get(), function);
    }

    template <typename UnaryFunction>
    static void post_order_impl(const Node* node, UnaryFunction& function) {
        if (node == nullptr) {
            return;
        }

        post_order_impl(node->left.get(), function);
        post_order_impl(node->right.get(), function);
        function(node->value);
    }

    bool is_valid_subtree(const Node* node, const T* lower, const T* upper) const {
        if (node == nullptr) {
            return true;
        }

        if (lower != nullptr && !compare_(*lower, node->value)) {
            return false;
        }

        if (upper != nullptr && !compare_(node->value, *upper)) {
            return false;
        }

        return is_valid_subtree(node->left.get(), lower, std::addressof(node->value)) &&
               is_valid_subtree(node->right.get(), std::addressof(node->value), upper);
    }
};

template <typename T, typename Compare>
void swap(BinarySearchTree<T, Compare>& lhs, BinarySearchTree<T, Compare>& rhs) {
    lhs.swap(rhs);
}

#endif
