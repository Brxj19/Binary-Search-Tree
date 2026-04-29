#include <iostream>

#include <bst/bst.h>

int main() {
    BinarySearchTree<int> tree = {8, 3, 10, 1, 6, 14, 4, 7, 13};

    const auto [iterator, inserted] = tree.insert(6);
    std::cout << "Inserted 6? " << std::boolalpha << inserted << '\n';
    std::cout << "Iterator points to: " << *iterator << '\n';

    tree.insert(5);
    tree.erase(10);

    std::cout << "Tree contents: ";
    for (const int value : tree) {
        std::cout << value << ' ';
    }
    std::cout << '\n';

    std::cout << "Contains 14? " << tree.contains(14) << '\n';
    std::cout << "Min: " << tree.min() << ", Max: " << tree.max() << '\n';
    return 0;
}
