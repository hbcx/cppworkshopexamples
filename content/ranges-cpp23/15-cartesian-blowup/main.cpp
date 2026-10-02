#include <ranges>
#include <vector>
#include <iostream>

// cartesian_product yields the PRODUCT of the input sizes. Three ranges of 50 is
// 125000 tuples; adding a fourth multiplies again. A small-looking expression can
// generate an enormous range, and materializing it allocates all of it.

int main() {
    std::vector<int> a(50), b(50), c(50);   // three modest-looking ranges

    // The combined range has 50 * 50 * 50 elements, from three lines that each
    // look small. Count it -- do NOT build a container from it.
    auto combos = std::views::cartesian_product(a, b, c);
    std::cout << "50 x 50 x 50 = " << std::ranges::distance(combos) << " tuples\n";

    // FIX: bound or filter BEFORE forcing the whole product, and never blindly
    // materialize a cartesian_product. Laziness means take(3) does almost no work.
    int taken = 0;
    for (auto t : combos | std::views::take(3)) { (void)t; ++taken; }
    std::cout << "took only " << taken << " instead of materializing all\n";
    return 0;
}
