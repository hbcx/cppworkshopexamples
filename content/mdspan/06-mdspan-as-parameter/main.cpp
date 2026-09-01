#include <mdspan>
#include <vector>
#include <array>
#include <iostream>
#include <cstddef>

// Any contiguous buffer, viewed as a 2D matrix of const int. Taken by value.
using Matrix = std::mdspan<const int, std::dextents<std::size_t, 2>>;

int sum_all(Matrix m) {
    int total = 0;
    for (std::size_t r = 0; r < m.extent(0); ++r)
        for (std::size_t c = 0; c < m.extent(1); ++c)
            total += m[r, c];
    return total;
}

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::array<int, 6> a{10, 20, 30, 40, 50, 60};
    int c_arr[6] = {100, 200, 300, 400, 500, 600};

    // One function, three different storages -- each viewed as a 2x3 matrix.
    std::cout << "sum(vector)  = " << sum_all(Matrix(v.data(), 2, 3)) << "\n";
    std::cout << "sum(array)   = " << sum_all(Matrix(a.data(), 2, 3)) << "\n";
    std::cout << "sum(C array) = " << sum_all(Matrix(c_arr, 2, 3)) << "\n";
}
