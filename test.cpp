#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm> // For std::is_sorted and std::equal

#include "bst.h" // The header we are testing

// A simple struct for testing with custom objects
struct TestObject {
    int id;
    std::string name;

    // FIX: Add a constructor to allow for emplacement
    TestObject(int i, std::string n) : id(i), name(std::move(n)) {}

    // We need comparison operators for the BST to work
    bool operator<(const TestObject& other) const {
        return id < other.id;
    }
    bool operator==(const TestObject& other) const {
        return id == other.id;
    }
};

// Test function declarations
void test_default_constructor_and_capacity();
void test_initializer_list_and_iterators();
void test_insert_find_contains();
void test_copy_semantics();
void test_move_semantics();
void test_erase();
void test_traversals();
void test_build_from_traversals();
void test_custom_object();
void test_clear();

int main() {
    // Sequentially run all test cases
    test_default_constructor_and_capacity();
    test_initializer_list_and_iterators();
    test_insert_find_contains();
    test_copy_semantics();
    test_move_semantics();
    test_erase();
    test_traversals();
    test_build_from_traversals();
    test_custom_object();
    test_clear();

    std::cout << "All BinarySearchTree tests passed successfully!" << std::endl;

    return 0;
}

void test_default_constructor_and_capacity() {
    std::cout << "Testing: Default Constructor and Capacity..." << std::endl;
    BinarySearchTree<int> bst;
    assert(bst.empty());
    assert(bst.size() == 0);
    assert(bst.begin() == bst.end());
}

void test_initializer_list_and_iterators() {
    std::cout << "Testing: Initializer List and Iterators..." << std::endl;
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 18};

    assert(!bst.empty());
    assert(bst.size() == 7);

    // Test iteration (should be in-order)
    std::vector<int> expected = {3, 5, 7, 10, 12, 15, 18};
    std::vector<int> actual;
    for (int val : bst) {
        actual.push_back(val);
    }
    assert(actual == expected);
    assert(std::is_sorted(actual.begin(), actual.end()));

    // Test bidirectional iteration
    auto it = bst.end();
    --it; // Should point to 18
    assert(*it == 18);
    --it; // Should point to 15
    assert(*it == 15);
    ++it; // Back to 18
    assert(*it == 18);
}

void test_insert_find_contains() {
    std::cout << "Testing: Insert, Find, and Contains..." << std::endl;
    BinarySearchTree<int> bst;
    
    // Insert
    auto result1 = bst.insert(20);
    assert(*result1 == 20);
    assert(bst.size() == 1);

    bst.insert(10);
    bst.insert(30);

    // Test insert of existing element
    auto result2 = bst.insert(20);
    assert(*result2 == 20); // Iterator should point to the existing element
    assert(bst.size() == 3); // Size should not change

    // Contains
    assert(bst.contains(10));
    assert(bst.contains(20));
    assert(bst.contains(30));
    assert(!bst.contains(99));

    // Find
    auto it_found = bst.find(10);
    assert(it_found != bst.end() && *it_found == 10);

    auto it_not_found = bst.find(99);
    assert(it_not_found == bst.end());

    // Const find
    const auto& const_bst = bst;
    auto const_it = const_bst.find(30);
    assert(const_it != const_bst.end() && *const_it == 30);
}


void test_copy_semantics() {
    std::cout << "Testing: Copy Semantics..." << std::endl;
    BinarySearchTree<int> bst1 = {10, 5, 15};

    // Copy constructor
    BinarySearchTree<int> bst2(bst1);
    assert(bst1.size() == 3);
    assert(bst2.size() == 3);
    assert(std::equal(bst1.begin(), bst1.end(), bst2.begin()));

    // Changes to bst2 should not affect bst1
    bst2.insert(20);
    assert(bst1.size() == 3);
    assert(bst2.size() == 4);
    assert(bst1.contains(15) && !bst1.contains(20));
    assert(bst2.contains(15) && bst2.contains(20));

    // Copy assignment
    BinarySearchTree<int> bst3;
    bst3 = bst1;
    assert(bst3.size() == 3);
    assert(std::equal(bst1.begin(), bst1.end(), bst3.begin()));
}

void test_move_semantics() {
    std::cout << "Testing: Move Semantics..." << std::endl;
    BinarySearchTree<int> bst1 = {10, 5, 15};

    // Move constructor
    BinarySearchTree<int> bst2(std::move(bst1));
    assert(bst1.empty()); // Original is empty after move
    assert(bst1.size() == 0);
    assert(bst2.size() == 3);
    assert(bst2.contains(10));

    // Move assignment
    BinarySearchTree<int> bst3;
    bst3 = std::move(bst2);
    assert(bst2.empty());
    assert(bst3.size() == 3);
    assert(bst3.contains(15));
}

void test_erase() {
    std::cout << "Testing: Erase..." << std::endl;
    BinarySearchTree<int> bst = {50, 30, 70, 20, 40, 60, 80, 35, 45};
    assert(bst.size() == 9);

    // Case 1: Erase a leaf node
    bst.erase(20);
    assert(!bst.contains(20));
    assert(bst.size() == 8);
    
    // Case 2: Erase a node with one child
    bst.erase(30); // Node 30 has right child 40
    assert(!bst.contains(30));
    assert(bst.find(40) != bst.end()); // 40 should be promoted
    assert(bst.size() == 7);
    
    // Case 3: Erase a node with two children
    bst.erase(50); // Erase the root
    assert(!bst.contains(50));
    assert(bst.size() == 6);
    // The successor of 50 is 60, so 60 should be the new root data
    // Let's verify the in-order traversal
    std::vector<int> expected = {35, 40, 45, 60, 70, 80};
    std::vector<int> actual;
    bst.in_order_traversal([&](int val){ actual.push_back(val); });
    assert(actual == expected);

    // Erase a non-existent element
    bst.erase(999);
    assert(bst.size() == 6);
}

void test_traversals() {
    std::cout << "Testing: Traversals..." << std::endl;
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 18};
    std::vector<int> result;
    
    // In-order
    bst.in_order_traversal([&](int val){ result.push_back(val); });
    assert(result == std::vector<int>({3, 5, 7, 10, 12, 15, 18}));
    result.clear();

    // Pre-order
    bst.pre_order_traversal([&](int val){ result.push_back(val); });
    assert(result == std::vector<int>({10, 5, 3, 7, 15, 12, 18}));
    result.clear();

    // Post-order
    bst.post_order_traversal([&](int val){ result.push_back(val); });
    assert(result == std::vector<int>({3, 7, 5, 12, 18, 15, 10}));
    result.clear();
}

void test_build_from_traversals() {
    std::cout << "Testing: Build From Traversals..." << std::endl;
    std::vector<int> preorder = {10, 5, 3, 7, 15, 12, 18};
    std::vector<int> inorder = {3, 5, 7, 10, 12, 15, 18};
    std::vector<int> postorder = {3, 7, 5, 12, 18, 15, 10};

    // Build from (pre-order, in-order)
    BinarySearchTree<int> bst1(preorder, inorder);
    assert(bst1.size() == 7);
    std::vector<int> postorder_result1;
    bst1.post_order_traversal([&](int val){ postorder_result1.push_back(val); });
    assert(postorder_result1 == postorder);

    // Build from (in-order, post-order)
    BinarySearchTree<int> bst2(inorder, postorder, true);
    assert(bst2.size() == 7);
    std::vector<int> preorder_result2;
    bst2.pre_order_traversal([&](int val){ preorder_result2.push_back(val); });
    assert(preorder_result2 == preorder);
}

void test_custom_object() {
    std::cout << "Testing: Custom Object..." << std::endl;
    BinarySearchTree<TestObject> bst;
    
    // This now works because TestObject has a matching constructor
    bst.emplace(10, "Apple"); 
    bst.emplace(5, "Banana"); 
    bst.emplace(15, "Cherry");

    assert(bst.size() == 3);
    assert(bst.contains({10, ""}));
    assert(!bst.contains({99, ""}));

    auto it = bst.find({5, ""});
    assert(it->name == "Banana");

    bst.erase({10, ""});
    assert(bst.size() == 2);
    assert(!bst.contains({10, ""}));
}

void test_clear() {
    std::cout << "Testing: Clear..." << std::endl;
    BinarySearchTree<int> bst = {10, 5, 15};
    assert(!bst.empty());
    
    bst.clear();
    assert(bst.empty());
    assert(bst.size() == 0);
    assert(bst.begin() == bst.end());
    
    // Check if it's usable after clearing
    bst.insert(100);
    assert(bst.size() == 1);
    assert(bst.contains(100));
}

