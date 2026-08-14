#include <algorithm>
#include <ranges>
#include <vector>
#include <string>
#include <utility>
#include <iostream>

// A type that owns data and provides its own efficient swap. Marking swap noexcept
// lets containers and algorithms rely on it (a vector reallocation, other swaps).
struct Buffer {
    std::string name;
    std::vector<int> data;

    friend void swap(Buffer& a, Buffer& b) noexcept {
        using std::swap;
        swap(a.name, b.name);
        swap(a.data, b.data);   // vector's O(1) member swap -- no deep copy
    }
};

int main() {
    Buffer x{"x", {1, 2, 3}};
    Buffer y{"y", {9, 8}};

    // Unqualified swap finds Buffer's own swap via ADL: it swaps the members, each
    // by their cheap swap, moving no vector elements.
    using std::swap;
    swap(x, y);
    std::cout << "x=" << x.name << " (" << x.data.size() << ") "
              << "y=" << y.name << " (" << y.data.size() << ")\n";

    // ranges::swap_ranges swaps two ranges taken directly, using each element
    // type's best swap -- here a range of Buffers.
    std::vector<Buffer> left{{"a", {1}}, {"b", {2}}};
    std::vector<Buffer> right{{"c", {3}}, {"d", {4}}};
    std::ranges::swap_ranges(left, right);
    std::cout << "left front now: " << left.front().name << '\n';   // c
    return 0;
}
