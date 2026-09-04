#include <iostream>
#include <vector>
#include <iterator>   // std::cbegin, std::cend, std::rbegin, std::crbegin (C++14)

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    int arr[] = {1, 2, 3, 4, 5};

    // cbegin/cend: read-only traversal even though v is not const. The iterator
    // is a const_iterator, so *it is const int& and the sequence cannot be
    // changed through it.
    long checksum = 0;
    for (auto it = std::cbegin(v); it != std::cend(v); ++it)
        checksum += *it;
    std::cout << "checksum = " << checksum << "\n";

    // rbegin/rend as free functions: walk backwards without hand-writing the
    // reverse loop.
    std::cout << "vector reversed:";
    for (auto it = std::rbegin(v); it != std::rend(v); ++it)
        std::cout << " " << *it;
    std::cout << "\n";

    // crbegin/crend: reverse AND read-only, and it works on the raw array too.
    std::cout << "array reversed: ";
    for (auto it = std::crbegin(arr); it != std::crend(arr); ++it)
        std::cout << " " << *it;
    std::cout << "\n";
    return 0;
}
