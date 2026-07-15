#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

// A plain C-style function: it knows nothing about std::vector, only a pointer
// to contiguous ints and a length.
static long sum_c_array(const int* data, std::size_t n) {
    long total = 0;
    for (std::size_t i = 0; i < n; ++i) {
        total += data[i];
    }
    return total;
}

int main() {
    std::vector<int> v(5);
    std::iota(v.begin(), v.end(), 1);   // fill with 1, 2, 3, 4, 5

    // vector storage is contiguous, so data() is a genuine array pointer.
    std::cout << "sum via C API = " << sum_c_array(v.data(), v.size()) << '\n';

    // "Contiguous" concretely means adjacent elements are one step apart in memory.
    const bool adjacent = (&v[1] - &v[0]) == 1;
    std::cout << "elements adjacent in memory? " << (adjacent ? "yes" : "no") << '\n';
    return 0;
}
