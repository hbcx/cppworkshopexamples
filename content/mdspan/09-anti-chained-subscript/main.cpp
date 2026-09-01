#include <mdspan>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::mdspan grid(v.data(), 2, 3);

    // TRAP: mdspan has no operator[] that returns a row you can index again,
    // so a chained subscript does not compile:
    //     int x = grid[1][2];   // ERROR: grid[1] is ill-formed (needs 2 indices)

    // FIX: one subscript with the whole index list (C++23 multidim subscript).
    int x = grid[1, 2];
    std::cout << "grid[1, 2] = " << x << "\n";   // 6

    // If you really want a row as its own 1D view, build it from the row start.
    std::mdspan row(v.data() + 1 * 3, 3);        // row 1: {4, 5, 6}
    std::cout << "row 1 as a 1D view: "
              << row[0] << ' ' << row[1] << ' ' << row[2] << "\n";
}
