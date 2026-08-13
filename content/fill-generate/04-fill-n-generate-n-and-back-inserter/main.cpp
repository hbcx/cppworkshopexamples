#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    // The _n variants take a START iterator and a COUNT instead of an end.
    std::vector<int> v(6, 0);
    std::fill_n(v.begin(), 3, 9);          // set the first three to 9
    std::cout << "fill_n first three: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // With a back_inserter the _n variants APPEND n values to an empty container,
    // building it without sizing it first. generate_n calls the generator n times.
    std::vector<int> built;
    int n = 1;
    std::generate_n(std::back_inserter(built), 5, [&n]() { return n++; });
    std::cout << "generate_n built:   ";
    for (int x : built) std::cout << x << ' ';
    std::cout << '\n';

    // fill_n + back_inserter appends n copies of a value.
    std::vector<int> pad;
    std::fill_n(std::back_inserter(pad), 4, -1);
    std::cout << "fill_n padded:      ";
    for (int x : pad) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
