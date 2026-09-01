#include <mdspan>
#include <vector>
#include <iostream>

int main() {
    // Data produced row-major -- logically
    //   1 2 3
    //   4 5 6
    // which in memory is 1,2,3,4,5,6.
    std::vector<int> mem{1, 2, 3, 4, 5, 6};

    // TRAP: the data is row-major, but we view it with layout_left
    // (column-major). No UB -- but the indices are wrong: [0,1] now maps to
    // memory position 2 (value 3), not the value 2 we meant.
    std::mdspan<int, std::extents<std::size_t, 2, 3>, std::layout_left> wrong(mem.data());
    std::cout << "wrong[0,1] = " << wrong[0, 1]
              << "  (meant 2, got the column-major element)\n";

    // FIX: match the layout to how the data was written. Row-major data needs
    // layout_right (the default).
    std::mdspan<int, std::extents<std::size_t, 2, 3>, std::layout_right> right(mem.data());
    std::cout << "right[0,1] = " << right[0, 1] << "  (correct)\n";
}
