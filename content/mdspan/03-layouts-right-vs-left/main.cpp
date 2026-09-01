#include <mdspan>
#include <vector>
#include <iostream>
#include <cstddef>

int main() {
    // Same six values in memory: 1,2,3,4,5,6.
    std::vector<int> mem{1, 2, 3, 4, 5, 6};

    // layout_right (the default): row-major, rows are contiguous.
    //   [0,0]=1 [0,1]=2 [0,2]=3 / [1,0]=4 [1,1]=5 [1,2]=6
    std::mdspan<int, std::extents<std::size_t, 2, 3>, std::layout_right> rr(mem.data());

    // layout_left: column-major, columns are contiguous (Fortran/BLAS order).
    //   [0,0]=1 [1,0]=2 [0,1]=3 [1,1]=4 [0,2]=5 [1,2]=6
    std::mdspan<int, std::extents<std::size_t, 2, 3>, std::layout_left> ll(mem.data());

    std::cout << "row-major (layout_right):\n";
    for (std::size_t r = 0; r < 2; ++r) {
        for (std::size_t c = 0; c < 3; ++c) std::cout << rr[r, c] << ' ';
        std::cout << '\n';
    }
    std::cout << "col-major (layout_left):\n";
    for (std::size_t r = 0; r < 2; ++r) {
        for (std::size_t c = 0; c < 3; ++c) std::cout << ll[r, c] << ' ';
        std::cout << '\n';
    }
}
