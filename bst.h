#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <memory>    // For std::unique_ptr
#include <stdexcept> // For std::out_of_range
#include <utility>   // For std::move, std::swap, std::forward
#include <cstddef>   // For std::size_t
#include <iterator>  // For std::bidirectional_iterator_tag
#include <stack>     // For implementing the iterator
#include <initializer_list> // For initializer_list constructor
#include <functional> // For std::function
#include <vector>    // For std::vector (used in new constructors)
#include <unordered_map> // For std::unordered_map (used in new constructors)

template <typename T>
class BinarySearchTree
{
private:
    struct Node
    {
        T data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node *parent; // Raw pointer to parent, doesn't own memory.

        // Constructor for creating nodes with a value
        explicit Node(const T &value, Node *p = nullptr)
            : data(value), left(nullptr), right(nullptr), parent(p) {}

        explicit Node(T &&value, Node *p = nullptr)
            : data(std::move(value)), left(nullptr), right(nullptr), parent(p) {}
        
        // Emplace constructor
        template<typename... Args>
        Node(Node* p, Args&&... args)
            : data(std::forward<Args>(args)...), left(nullptr), right(nullptr), parent(p) {}
    };

    std::unique_ptr<Node> root_;
    std::size_t tree_size_;

public:
    template<typename U> friend class base_iterator; // Allow iterator to access private members

    // =======================================================================
    // == ITERATOR
    // =======================================================================
    // A BST iterator performs an in-order traversal to visit nodes in sorted order.
    template<typename U>
    class base_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = U;
        using difference_type = std::ptrdiff_t;
        using pointer = U*;
        using reference = U&;

    private:
        Node* ptr_;
        const BinarySearchTree<T>* tree_; // Pointer back to the container

        // Helper to find the next in-order node (successor)
        Node* successor() {
            if (ptr_->right) {
                ptr_ = ptr_->right.get();
                while (ptr_->left) {
                    ptr_ = ptr_->left.get();
                }
                return ptr_;
            }
            Node* p = ptr_->parent;
            while(p && ptr_ == p->right.get()) {
                ptr_ = p;
                p = p->parent;
            }
            ptr_ = p;
            return ptr_;
        }

        // Helper to find the previous in-order node (predecessor)
        Node* predecessor() {
            if (ptr_->left) {
                ptr_ = ptr_->left.get();
                while (ptr_->right) {
                    ptr_ = ptr_->right.get();
                }
                return ptr_;
            }
             Node* p = ptr_->parent;
            while(p && ptr_ == p->left.get()) {
                ptr_ = p;
                p = p->parent;
            }
            ptr_ = p;
            return ptr_;
        }
    
    public:
        explicit base_iterator(Node *p = nullptr, const BinarySearchTree<T>* t = nullptr) : ptr_(p), tree_(t) {}
        
        // Implicit conversion from iterator to const_iterator
        template<typename V>
        base_iterator(const base_iterator<V>& other) : ptr_(other.ptr_), tree_(other.tree_) {}


        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &(ptr_->data); }

        base_iterator& operator++() { // Pre-increment
            successor();
            return *this;
        }

        base_iterator operator++(int) { // Post-increment
            base_iterator tmp = *this;
            successor();
            return tmp;
        }

        base_iterator& operator--() { // Pre-decrement
            if (!ptr_) { // Special case for end() iterator
                if (tree_ && !tree_->empty()) {
                    ptr_ = tree_->find_max(tree_->root_.get());
                }
                return *this;
            }
            predecessor();
            return *this;
        }
        
        base_iterator operator--(int) { // Post-decrement
             base_iterator tmp = *this;
            predecessor();
            return tmp;
        }

        bool operator==(const base_iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const base_iterator& other) const { return ptr_ != other.ptr_; }

        // Make iterator and const_iterator friends for conversion
        template<typename V>
        friend class base_iterator;
    };

    using iterator = base_iterator<T>;
    using const_iterator = base_iterator<const T>;

    // =======================================================================
    // == LIFECYCLE
    // =======================================================================
    BinarySearchTree() noexcept : root_(nullptr), tree_size_(0) {}
    ~BinarySearchTree() = default; // unique_ptr handles cleanup

    // Copy constructor
    BinarySearchTree(const BinarySearchTree &other) : root_(nullptr), tree_size_(0) {
        if (other.root_) {
            root_ = std::make_unique<Node>(other.root_->data);
            copy_helper(root_.get(), other.root_.get());
            tree_size_ = other.tree_size_;
        }
    }

    // Copy assignment
    BinarySearchTree &operator=(BinarySearchTree other) noexcept {
        swap(*this, other);
        return *this;
    }

    // Move constructor
    BinarySearchTree(BinarySearchTree &&other) noexcept
        : root_(std::move(other.root_)), tree_size_(other.tree_size_) {
        other.tree_size_ = 0;
    }

    // Initializer list constructor
    BinarySearchTree(std::initializer_list<T> ilist) : BinarySearchTree() {
        for(const auto& val : ilist) {
            insert(val);
        }
    }
    
    // Constructor from Pre-order and In-order traversals
    BinarySearchTree(const std::vector<T>& preorder, const std::vector<T>& inorder) : BinarySearchTree() {
        if (preorder.empty() || preorder.size() != inorder.size()) {
            return; // Or throw an exception for invalid input
        }
        std::unordered_map<T, int> inorder_map;
        for (int i = 0; i < inorder.size(); ++i) {
            inorder_map[inorder[i]] = i;
        }
        int pre_idx = 0;
        root_ = build_from_pre_in(preorder, pre_idx, inorder_map, 0, inorder.size() - 1, nullptr);
        tree_size_ = preorder.size();
    }

    // Constructor from In-order and Post-order traversals
    // A dummy bool is used to differentiate the signature from the (pre, in) constructor.
    BinarySearchTree(const std::vector<T>& inorder, const std::vector<T>& postorder, bool /*dummy*/) : BinarySearchTree() {
         if (postorder.empty() || postorder.size() != inorder.size()) {
            return; // Or throw an exception for invalid input
        }
        std::unordered_map<T, int> inorder_map;
        for (int i = 0; i < inorder.size(); ++i) {
            inorder_map[inorder[i]] = i;
        }
        int post_idx = postorder.size() - 1;
        root_ = build_from_post_in(postorder, post_idx, inorder_map, 0, inorder.size() - 1, nullptr);
        tree_size_ = postorder.size();
    }


    friend void swap(BinarySearchTree &a, BinarySearchTree &b) noexcept {
        using std::swap;
        swap(a.root_, b.root_);
        swap(a.tree_size_, b.tree_size_);
    }

    // =======================================================================
    // == CAPACITY
    // =======================================================================
    std::size_t size() const noexcept { return tree_size_; }
    bool empty() const noexcept { return tree_size_ == 0; }

    // =======================================================================
    // == MODIFIERS
    // =======================================================================

    // Insert (copy)
    iterator insert(const T &value) {
        return emplace(value).first;
    }

    // Insert (move)
    iterator insert(T &&value) {
        return emplace(std::move(value)).first;
    }
    
    // Emplace (construct in-place)
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        // Create a temporary object to find the correct insertion spot
        T val(std::forward<Args>(args)...);

        if (!root_) {
            root_ = std::make_unique<Node>(nullptr, std::move(val));
            tree_size_++;
            return {iterator(root_.get(), this), true};
        }

        Node* current = root_.get();
        Node* parent = nullptr;
        while(current) {
            parent = current;
            if (val < current->data) {
                current = current->left.get();
            } else if (current->data < val) {
                current = current->right.get();
            } else {
                return {iterator(current, this), false}; // Value already exists
            }
        }
        
        Node* new_node;
        if (val < parent->data) {
            parent->left = std::make_unique<Node>(parent, std::move(val));
            new_node = parent->left.get();
        } else {
            parent->right = std::make_unique<Node>(parent, std::move(val));
            new_node = parent->right.get();
        }

        tree_size_++;
        return {iterator(new_node, this), true};
    }

    // Erase by key
    iterator erase(const T &key) {
        Node* node_to_erase = find_node(root_.get(), key);
        if (!node_to_erase) {
            return end(); // Not found
        }
        
        iterator next_node_it(node_to_erase, this);
        ++next_node_it; // Find the successor before deleting

        // Case 1: Node is a leaf
        if (!node_to_erase->left && !node_to_erase->right) {
            detach_from_parent(node_to_erase);
        }
        // Case 2: Node has one child
        else if (!node_to_erase->left || !node_to_erase->right) {
            std::unique_ptr<Node>& child = node_to_erase->left ? node_to_erase->left : node_to_erase->right;
            child->parent = node_to_erase->parent;
            
            // This move operation transfers ownership of the child subtree
            // to the parent of the node being erased.
            get_parent_link(node_to_erase) = std::move(child);
        }
        // Case 3: Node has two children
        else {
            Node* successor = find_min(node_to_erase->right.get());
            // We can't just move the data if T is not assignable or complex.
            // The robust way is to replace the node, but that's much more complex.
            // A common and simpler (though potentially less efficient for complex types)
            // way is to swap data. For this library, we'll assume T is movable.
            
            // This is a simplification. A fully robust erase would move nodes, not data.
            node_to_erase->data = std::move(successor->data);
            
            // Now erase the successor (which has at most one right child)
            erase_node_recursive(node_to_erase->right, successor->data);
            // This is a recursive call to handle the actual node removal, 
            // but we've already found our return iterator, so this is okay.
            tree_size_++; // Re-adjust size because recursive call will decrement it.
        }

        tree_size_--;
        return next_node_it;
    }

    void clear() noexcept {
        root_.reset();
        tree_size_ = 0;
    }

    // =======================================================================
    // == LOOKUP
    // =======================================================================
    iterator find(const T &key) {
        return iterator(find_node(root_.get(), key), this);
    }

    const_iterator find(const T &key) const {
        return const_iterator(find_node(root_.get(), key), this);
    }

    bool contains(const T &key) const {
        return find_node(root_.get(), key) != nullptr;
    }

    // =======================================================================
    // == ITERATORS
    // =======================================================================
    iterator begin() {
        return iterator(find_min(root_.get()), this);
    }
    const_iterator begin() const {
        return const_iterator(find_min(root_.get()), this);
    }
    const_iterator cbegin() const {
        return const_iterator(find_min(root_.get()), this);
    }

    iterator end() {
        return iterator(nullptr, this);
    }
    const_iterator end() const {
        return const_iterator(nullptr, this);
    }
    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }

    // =======================================================================
    // == TRAVERSALS
    // =======================================================================
    
    // Performs an in-order traversal (Left, Root, Right), visiting nodes in sorted order.
    void in_order_traversal(std::function<void(const T&)> func) const {
        in_order_helper(root_.get(), func);
    }

    // Performs a pre-order traversal (Root, Left, Right).
    void pre_order_traversal(std::function<void(const T&)> func) const {
        pre_order_helper(root_.get(), func);
    }

    // Performs a post-order traversal (Left, Right, Root).
    void post_order_traversal(std::function<void(const T&)> func) const {
        post_order_helper(root_.get(), func);
    }

private:
    // =======================================================================
    // == PRIVATE HELPERS
    // =======================================================================

    // Helper for copy constructor
    void copy_helper(Node* parent_copy, Node* other_node) {
        if (other_node->left) {
            parent_copy->left = std::make_unique<Node>(other_node->left->data, parent_copy);
            copy_helper(parent_copy->left.get(), other_node->left.get());
        }
        if (other_node->right) {
            parent_copy->right = std::make_unique<Node>(other_node->right->data, parent_copy);
            copy_helper(parent_copy->right.get(), other_node->right.get());
        }
    }
    
    // Find node with a given key
    Node* find_node(Node* current, const T &key) const {
        while(current) {
            if (key < current->data) {
                current = current->left.get();
            } else if (current->data < key) {
                current = current->right.get();
            } else {
                return current;
            }
        }
        return nullptr;
    }

    // Find the node with the minimum value in a subtree
    Node* find_min(Node* node) const {
        if (!node) return nullptr;
        while(node->left) {
            node = node->left.get();
        }
        return node;
    }

    // Find the node with the maximum value in a subtree
    Node* find_max(Node* node) const {
        if (!node) return nullptr;
        while(node->right) {
            node = node->right.get();
        }
        return node;
    }

    // Helper to get the unique_ptr in the parent that points to `node`.
    std::unique_ptr<Node>& get_parent_link(Node* node) {
        if (!node->parent) return root_;
        if (node->parent->left.get() == node) return node->parent->left;
        return node->parent->right;
    }

    void detach_from_parent(Node* node) {
        get_parent_link(node).reset();
    }

    // A recursive helper for the complex erase case.
    void erase_node_recursive(std::unique_ptr<Node>& current_node, const T& key) {
        if (!current_node) return;

        if (key < current_node->data) {
            erase_node_recursive(current_node->left, key);
        } else if (current_node->data < key) {
            erase_node_recursive(current_node->right, key);
        } else {
            // Found the node (successor) to erase
            if (!current_node->left || !current_node->right) {
                std::unique_ptr<Node>& child = current_node->left ? current_node->left : current_node->right;
                if(child) child->parent = current_node->parent;
                current_node = std::move(child);
                tree_size_--;
            }
            // The two-child case is handled in the public erase function
        }
    }

    // Traversal helpers
    void in_order_helper(Node* node, std::function<void(const T&)>& func) const {
        if (!node) return;
        in_order_helper(node->left.get(), func);
        func(node->data);
        in_order_helper(node->right.get(), func);
    }

    void pre_order_helper(Node* node, std::function<void(const T&)>& func) const {
        if (!node) return;
        func(node->data);
        pre_order_helper(node->left.get(), func);
        pre_order_helper(node->right.get(), func);
    }

    void post_order_helper(Node* node, std::function<void(const T&)>& func) const {
        if (!node) return;
        post_order_helper(node->left.get(), func);
        post_order_helper(node->right.get(), func);
        func(node->data);
    }

    // Helper for building tree from Pre-order and In-order
    std::unique_ptr<Node> build_from_pre_in(const std::vector<T>& preorder, int& pre_idx,
                                            const std::unordered_map<T, int>& inorder_map,
                                            int in_start, int in_end, Node* parent) {
        if (in_start > in_end) {
            return nullptr;
        }

        T root_val = preorder[pre_idx++];
        auto node = std::make_unique<Node>(root_val, parent);

        int in_root_idx = inorder_map.at(root_val);

        node->left = build_from_pre_in(preorder, pre_idx, inorder_map, in_start, in_root_idx - 1, node.get());
        node->right = build_from_pre_in(preorder, pre_idx, inorder_map, in_root_idx + 1, in_end, node.get());

        return node;
    }
    
    // Helper for building tree from Post-order and In-order
    std::unique_ptr<Node> build_from_post_in(const std::vector<T>& postorder, int& post_idx,
                                             const std::unordered_map<T, int>& inorder_map,
                                             int in_start, int in_end, Node* parent) {
        if (in_start > in_end) {
            return nullptr;
        }

        T root_val = postorder[post_idx--];
        auto node = std::make_unique<Node>(root_val, parent);

        int in_root_idx = inorder_map.at(root_val);
        
        // Note: Build right subtree first because we are iterating postorder from the end
        node->right = build_from_post_in(postorder, post_idx, inorder_map, in_root_idx + 1, in_end, node.get());
        node->left = build_from_post_in(postorder, post_idx, inorder_map, in_start, in_root_idx - 1, node.get());

        return node;
    }
};

#endif // BINARY_SEARCH_TREE_H


