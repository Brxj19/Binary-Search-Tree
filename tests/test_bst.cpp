#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <bst/bst.h>

struct Record {
    int id;
    std::string name;

    Record(int new_id, std::string new_name) : id(new_id), name(std::move(new_name)) {}

    bool operator<(const Record& other) const {
        return id < other.id;
    }
};

struct DescendingInt {
    bool operator()(int lhs, int rhs) const {
        return lhs > rhs;
    }
};

void test_default_constructor();
void test_initializer_list_constructor();
void test_range_constructor();
void test_insert_and_duplicate_insert();
void test_emplace();
void test_find_and_contains();
void test_erase_leaf();
void test_erase_one_child();
void test_erase_two_children();
void test_erase_root();
void test_erase_missing_and_empty();
void test_string_values();
void test_custom_object_values();
void test_custom_comparator();
void test_iterator_traversal();
void test_const_iterator_traversal();
void test_begin_end_on_empty_tree();
void test_clear_and_reinsert();
void test_copy_constructor();
void test_copy_assignment();
void test_move_constructor();
void test_move_assignment();
void test_traversal_methods();
void test_min_max_height_to_vector();
void test_bounds_and_validity();

int main() {
    test_default_constructor();
    test_initializer_list_constructor();
    test_range_constructor();
    test_insert_and_duplicate_insert();
    test_emplace();
    test_find_and_contains();
    test_erase_leaf();
    test_erase_one_child();
    test_erase_two_children();
    test_erase_root();
    test_erase_missing_and_empty();
    test_string_values();
    test_custom_object_values();
    test_custom_comparator();
    test_iterator_traversal();
    test_const_iterator_traversal();
    test_begin_end_on_empty_tree();
    test_clear_and_reinsert();
    test_copy_constructor();
    test_copy_assignment();
    test_move_constructor();
    test_move_assignment();
    test_traversal_methods();
    test_min_max_height_to_vector();
    test_bounds_and_validity();

    std::cout << "All BinarySearchTree tests passed." << std::endl;
    return 0;
}

void test_default_constructor() {
    BinarySearchTree<int> bst;
    assert(bst.empty());
    assert(bst.size() == 0);
    assert(bst.begin() == bst.end());
    assert(bst.cbegin() == bst.cend());
}

void test_initializer_list_constructor() {
    BinarySearchTree<int> bst = {10, 5, 15, 5, 12};
    assert(!bst.empty());
    assert(bst.size() == 4);
    assert(bst.to_vector() == std::vector<int>({5, 10, 12, 15}));
}

void test_range_constructor() {
    const std::vector<int> values = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    BinarySearchTree<int> bst(values.begin(), values.end());
    assert(bst.size() == values.size());
    assert(bst.to_vector() == std::vector<int>({1, 3, 4, 6, 7, 8, 10, 13, 14}));
}

void test_insert_and_duplicate_insert() {
    BinarySearchTree<int> bst;

    const auto first = bst.insert(42);
    assert(first.second);
    assert(*first.first == 42);
    assert(bst.size() == 1);

    const auto duplicate = bst.insert(42);
    assert(!duplicate.second);
    assert(*duplicate.first == 42);
    assert(bst.size() == 1);

    bst.insert(10);
    bst.insert(50);
    assert(bst.to_vector() == std::vector<int>({10, 42, 50}));
}

void test_emplace() {
    BinarySearchTree<Record> bst;

    const auto inserted = bst.emplace(7, "seven");
    assert(inserted.second);
    assert(inserted.first->id == 7);
    assert(inserted.first->name == "seven");

    const auto duplicate = bst.emplace(7, "ignored");
    assert(!duplicate.second);
    assert(duplicate.first->name == "seven");
}

void test_find_and_contains() {
    BinarySearchTree<int> bst = {20, 10, 30, 25};

    assert(bst.contains(10));
    assert(!bst.contains(99));

    const auto found = bst.find(25);
    assert(found != bst.end());
    assert(*found == 25);

    const auto missing = bst.find(99);
    assert(missing == bst.end());
}

void test_erase_leaf() {
    BinarySearchTree<int> bst = {20, 10, 30, 5, 15};
    assert(bst.erase(5) == 1);
    assert(!bst.contains(5));
    assert(bst.to_vector() == std::vector<int>({10, 15, 20, 30}));
    assert(bst.is_valid_bst());
}

void test_erase_one_child() {
    BinarySearchTree<int> bst = {20, 10, 30, 25};
    assert(bst.erase(30) == 1);
    assert(!bst.contains(30));
    assert(bst.contains(25));
    assert(bst.to_vector() == std::vector<int>({10, 20, 25}));
    assert(bst.is_valid_bst());
}

void test_erase_two_children() {
    BinarySearchTree<int> bst = {50, 30, 70, 20, 40, 60, 80, 35, 45};
    assert(bst.erase(30) == 1);
    assert(!bst.contains(30));
    assert(bst.to_vector() == std::vector<int>({20, 35, 40, 45, 50, 60, 70, 80}));
    assert(bst.is_valid_bst());

    auto position = bst.find(50);
    auto next = bst.erase(position);
    assert(next != bst.end());
    assert(*next == 60);
    assert(!bst.contains(50));
    assert(bst.to_vector() == std::vector<int>({20, 35, 40, 45, 60, 70, 80}));
    assert(bst.is_valid_bst());
}

void test_erase_root() {
    BinarySearchTree<int> bst = {10, 5, 15, 12, 18};
    assert(bst.erase(10) == 1);
    assert(!bst.contains(10));
    assert(bst.to_vector() == std::vector<int>({5, 12, 15, 18}));
    assert(bst.is_valid_bst());
}

void test_erase_missing_and_empty() {
    BinarySearchTree<int> bst;
    assert(bst.erase(123) == 0);
    assert(bst.erase(bst.end()) == bst.end());

    bst.insert(1);
    assert(bst.erase(999) == 0);
    assert(bst.size() == 1);
}

void test_string_values() {
    BinarySearchTree<std::string> bst = {"pear", "apple", "orange"};
    assert(bst.contains("apple"));
    assert(bst.erase("pear") == 1);
    assert(bst.to_vector() == std::vector<std::string>({"apple", "orange"}));
}

void test_custom_object_values() {
    BinarySearchTree<Record> bst;
    bst.emplace(10, "ten");
    bst.emplace(4, "four");
    bst.emplace(12, "twelve");

    const auto found = bst.find(Record{4, "unused"});
    assert(found != bst.end());
    assert(found->name == "four");

    assert(bst.erase(Record{10, "unused"}) == 1);
    assert(!bst.contains(Record{10, "unused"}));
    assert(bst.is_valid_bst());
}

void test_custom_comparator() {
    BinarySearchTree<int, DescendingInt> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(20);
    bst.insert(15);

    assert(bst.to_vector() == std::vector<int>({20, 15, 10, 5}));
    assert(*bst.lower_bound(16) == 15);
    assert(*bst.upper_bound(15) == 10);
    assert(bst.is_valid_bst());
}

void test_iterator_traversal() {
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7};
    std::vector<int> values;

    for (auto it = bst.begin(); it != bst.end(); ++it) {
        values.push_back(*it);
    }

    assert(values == std::vector<int>({3, 5, 7, 10, 15}));

    auto it = bst.end();
    --it;
    assert(*it == 15);
    --it;
    assert(*it == 10);
}

void test_const_iterator_traversal() {
    const BinarySearchTree<int> bst = {10, 5, 15};
    std::vector<int> values;

    for (auto it = bst.cbegin(); it != bst.cend(); ++it) {
        values.push_back(*it);
    }

    assert(values == std::vector<int>({5, 10, 15}));
}

void test_begin_end_on_empty_tree() {
    BinarySearchTree<int> bst;
    assert(bst.begin() == bst.end());
    assert(bst.cbegin() == bst.cend());
}

void test_clear_and_reinsert() {
    BinarySearchTree<int> bst = {1, 2, 3};
    bst.clear();
    assert(bst.empty());

    bst.insert(8);
    bst.insert(4);
    assert(bst.to_vector() == std::vector<int>({4, 8}));
}

void test_copy_constructor() {
    BinarySearchTree<int> original = {9, 3, 11};
    BinarySearchTree<int> copy(original);

    assert(copy.to_vector() == original.to_vector());
    copy.insert(12);
    assert(!original.contains(12));
    assert(copy.contains(12));
}

void test_copy_assignment() {
    BinarySearchTree<int> source = {1, 4, 9};
    BinarySearchTree<int> target;
    target = source;

    assert(target.to_vector() == source.to_vector());
    target.erase(4);
    assert(source.contains(4));
}

void test_move_constructor() {
    BinarySearchTree<int> source = {2, 1, 3};
    BinarySearchTree<int> moved(std::move(source));

    assert(moved.to_vector() == std::vector<int>({1, 2, 3}));
    assert(source.empty());
}

void test_move_assignment() {
    BinarySearchTree<int> source = {6, 5, 7};
    BinarySearchTree<int> target;
    target = std::move(source);

    assert(target.to_vector() == std::vector<int>({5, 6, 7}));
    assert(source.empty());
}

void test_traversal_methods() {
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 18};
    std::vector<int> in_order;
    std::vector<int> pre_order;
    std::vector<int> post_order;

    bst.in_order_traversal([&in_order](const int value) { in_order.push_back(value); });
    bst.pre_order_traversal([&pre_order](const int value) { pre_order.push_back(value); });
    bst.post_order_traversal([&post_order](const int value) { post_order.push_back(value); });

    assert(in_order == std::vector<int>({3, 5, 7, 10, 12, 15, 18}));
    assert(pre_order == std::vector<int>({10, 5, 3, 7, 15, 12, 18}));
    assert(post_order == std::vector<int>({3, 7, 5, 12, 18, 15, 10}));
}

void test_min_max_height_to_vector() {
    BinarySearchTree<int> bst = {10, 5, 15, 3, 7, 12, 18};
    assert(bst.min() == 3);
    assert(bst.max() == 18);
    assert(bst.height() == 3);
    assert(bst.to_vector() == std::vector<int>({3, 5, 7, 10, 12, 15, 18}));

    BinarySearchTree<int> empty;
    bool min_threw = false;
    bool max_threw = false;

    try {
        (void)empty.min();
    } catch (const std::out_of_range&) {
        min_threw = true;
    }

    try {
        (void)empty.max();
    } catch (const std::out_of_range&) {
        max_threw = true;
    }

    assert(min_threw);
    assert(max_threw);
}

void test_bounds_and_validity() {
    BinarySearchTree<int> bst = {20, 10, 30, 25, 40};

    assert(*bst.lower_bound(24) == 25);
    assert(*bst.lower_bound(25) == 25);
    assert(*bst.upper_bound(25) == 30);
    assert(bst.lower_bound(99) == bst.end());
    assert(bst.upper_bound(40) == bst.end());
    assert(bst.is_valid_bst());
}
