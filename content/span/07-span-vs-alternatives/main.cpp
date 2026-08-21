#include <array>
#include <cstddef>
#include <iostream>
#include <span>
#include <vector>

// 1) The old C way: a pointer and a size. Works with anything, but the two
//    arguments can disagree, and nothing stops out-of-range access.
long sum_ptr(const int* data, std::size_t n) {
    long t = 0;
    for (std::size_t i = 0; i < n; ++i) t += data[i];
    return t;
}

// 2) A specific container: safe and simple, but ONLY a std::vector -- an array
//    or C array must be copied into one first.
long sum_vec(const std::vector<int>& v) {
    long t = 0;
    for (int x : v) t += x;
    return t;
}

// 3) span: one argument, any contiguous source, size travels with the data.
long sum_span(std::span<const int> s) {
    long t = 0;
    for (int x : s) t += x;
    return t;
}

int main() {
    std::array<int, 3> a{1, 2, 3};

    // sum_ptr and sum_span take the array directly; sum_vec needs a copy first.
    std::cout << "ptr:         " << sum_ptr(a.data(), a.size()) << '\n';
    std::cout << "span:        " << sum_span(a) << '\n';
    std::cout << "vec (copied):" << sum_vec(std::vector<int>(a.begin(), a.end())) << '\n';

    std::vector<int> v{4, 5, 6, 7};
    std::cout << "span vector: " << sum_span(v) << '\n';
    return 0;
}
