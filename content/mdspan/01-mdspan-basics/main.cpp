#include <mdspan>
#include <vector>
#include <iostream>
#include <cstddef>

int main() {
    // A flat buffer of 6 ints, laid out as a 2x3 grid, row by row.
    std::vector<int> storage{10, 20, 30, 40, 50, 60};

    // A 2-by-3 view over that buffer. mdspan does not own or copy the data;
    // it is a pointer plus the shape (here two runtime extents: 2 and 3).
    std::mdspan grid(storage.data(), 2, 3);

    std::cout << "rank      = " << grid.rank() << "\n";       // 2
    std::cout << "extent(0) = " << grid.extent(0) << "\n";    // rows = 2
    std::cout << "extent(1) = " << grid.extent(1) << "\n";    // cols = 3
    std::cout << "size      = " << grid.size() << "\n";       // 6

    // Multidimensional subscript (C++23): one [] with a comma-separated list.
    for (std::size_t r = 0; r < grid.extent(0); ++r) {
        for (std::size_t c = 0; c < grid.extent(1); ++c) {
            std::cout << grid[r, c] << ' ';
        }
        std::cout << '\n';
    }

    // Writing through the view changes the underlying buffer.
    grid[0, 0] = 99;
    std::cout << "storage[0] after grid[0,0]=99 -> " << storage[0] << "\n";
}
