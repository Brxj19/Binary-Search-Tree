#include <iostream>
#include <vector>

#include <bst/bst.h>

int main() {
    BinarySearchTree<int> tree = {10, 5, 15, 3, 7, 12, 18};

    std::cout << "In-order: ";
    tree.in_order_traversal([](const int value) {
        std::cout << value << ' ';
    });
    std::cout << '\n';

    std::cout << "Pre-order: ";
    tree.pre_order_traversal([](const int value) {
        std::cout << value << ' ';
    });
    std::cout << '\n';

    std::cout << "Post-order: ";
    tree.post_order_traversal([](const int value) {
        std::cout << value << ' ';
    });
    std::cout << '\n';

    const std::vector<int> sorted = tree.to_vector();
    std::cout << "Vector size: " << sorted.size() << '\n';
    return 0;
}
