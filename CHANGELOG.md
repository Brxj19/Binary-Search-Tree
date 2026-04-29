# Changelog

All notable changes to this project will be documented in this file.

## [Unreleased]

### Added

- Public library structure with `include/`, `examples/`, `tests/`, `docs/`, and CMake support
- Comparator-aware `BinarySearchTree<T, Compare>`
- STL-style `insert` and `emplace` return values
- `lower_bound`, `upper_bound`, `min`, `max`, `height`, `to_vector`, and `is_valid_bst`
- Expanded test suite, buildable examples, and GitHub Actions CI
- Doxygen-style comments in the public header
- Public-facing documentation and GitHub Pages entry page

### Changed

- Moved the main public header to `include/bst/bst.h`
- Kept a root-level `bst.h` compatibility wrapper
- Improved erase correctness for two-child deletion without moving through a temporary key
