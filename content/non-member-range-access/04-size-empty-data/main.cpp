#include <iostream>
#include <vector>
#include <iterator>   // std::size, std::empty, std::data (C++17)
#include <cstddef>    // std::size_t

// A C-style API: pointer plus length, no idea what a std::vector is.
static long sum_buffer(const int* p, std::size_t n) {
    long total = 0;
    for (std::size_t i = 0; i < n; ++i) total += p[i];
    return total;
}

int main() {
    std::vector<int> v = {5, 10, 15, 20};
    int arr[] = {1, 2, 3};

    // std::size: element count, one spelling for both. For the array it is the
    // length 3 -- no sizeof(arr)/sizeof(arr[0]) trick.
    std::cout << "size(vector) = " << std::size(v) << "\n";
    std::cout << "size(array)  = " << std::size(arr) << "\n";

    // std::empty: works on containers and arrays (a C array is never empty).
    std::vector<int> none;
    std::cout << std::boolalpha << "empty(none) = " << std::empty(none) << "\n";

    // std::data + std::size: pass the contiguous buffer to a C API without the
    // fragile &v[0].
    std::cout << "sum via C API = " << sum_buffer(std::data(v), std::size(v)) << "\n";
    return 0;
}
