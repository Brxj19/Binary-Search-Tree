#include <iostream>

#include <bst/bst.h>

int main() {
    BinarySearchTree<int> tree = {20, 10, 30, 25, 40, 22};

    std::cout << "Forward: ";
    for (auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    std::cout << "Reverse: ";
    for (auto it = tree.end(); it != tree.begin();) {
        --it;
        std::cout << *it << ' ';
    }
    std::cout << '\n';

    auto lower = tree.lower_bound(23);
    auto upper = tree.upper_bound(25);
    std::cout << "lower_bound(23): " << *lower << '\n';
    std::cout << "upper_bound(25): " << *upper << '\n';

    auto next = tree.erase(tree.find(25));
    std::cout << "After erasing 25, next element is ";
    if (next != tree.end()) {
        std::cout << *next << '\n';
    } else {
        std::cout << "end()\n";
    }

    return 0;
}
