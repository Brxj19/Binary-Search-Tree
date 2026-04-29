#include <iostream>
#include <string>

#include <bst/bst.h>

struct Book {
    std::string title;
    int year;

    Book(std::string new_title, int new_year)
        : title(std::move(new_title)), year(new_year) {}
};

struct BookTitleLess {
    bool operator()(const Book& lhs, const Book& rhs) const {
        return lhs.title < rhs.title;
    }
};

int main() {
    BinarySearchTree<Book, BookTitleLess> library;
    library.emplace("The Hobbit", 1937);
    library.emplace("Dune", 1965);
    library.emplace("Foundation", 1951);

    const auto found = library.find(Book{"Dune", 0});
    if (found != library.end()) {
        std::cout << found->title << " (" << found->year << ")\n";
    }

    for (const auto& book : library) {
        std::cout << book.title << '\n';
    }

    return 0;
}
