#include <mdspan>
#include <vector>
#include <array>
#include <iostream>
#include <cstddef>

int main() {
    // A 4x4 matrix stored row-major: values 0..15.
    std::vector<int> big(16);
    for (int i = 0; i < 16; ++i) big[i] = i;

    // We want the inner 2x2 block starting at row 1, col 1:
    //    5  6
    //    9 10
    // Its top-left is at flat offset 1*4 + 1 = 5. To step one full matrix row
    // between block rows the row stride is 4 (the parent width); the column
    // stride is 1. layout_stride carries those two strides explicitly.
    using Ext = std::extents<std::size_t, 2, 2>;
    std::layout_stride::mapping<Ext> map{Ext{}, std::array<std::size_t, 2>{4, 1}};
    std::mdspan<int, Ext, std::layout_stride> block(big.data() + 5, map);

    std::cout << "inner 2x2 block:\n";
    for (std::size_t r = 0; r < 2; ++r) {
        for (std::size_t c = 0; c < 2; ++c) std::cout << block[r, c] << ' ';
        std::cout << '\n';
    }

    // Writing through the sub-view edits the parent matrix in place.
    block[0, 0] = 500;
    std::cout << "big[5] after block[0,0]=500 -> " << big[5] << "\n";
}
