#include <array>
#include <iostream>
#include <span>
#include <vector>

// One function that reads ANY contiguous sequence of ints, with no copy and no
// template. Before span you needed a template, a (pointer, size) pair, or an
// overload per container type.
long sum(std::span<const int> values) {
    long total = 0;
    for (int v : values) total += v;
    return total;
}

int main() {
    std::vector<int> vec{1, 2, 3, 4};
    std::array<int, 3> arr{10, 20, 30};
    int c[] = {100, 200};

    // All three call the same function; each converts to a span implicitly.
    std::cout << "vector:  " << sum(vec) << '\n';
    std::cout << "array:   " << sum(arr) << '\n';
    std::cout << "C array: " << sum(c) << '\n';

    // A slice works too, with no copy: view part of the vector via subspan.
    std::cout << "middle:  " << sum(std::span<const int>(vec).subspan(1, 2)) << '\n';
    return 0;
}
