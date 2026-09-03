#include <functional>
#include <iostream>
#include <vector>

int main() {
    int x = 1, y = 2, z = 3;
    std::vector<std::reference_wrapper<int>> original{x, y, z};

    // ANTI-PATTERN: expecting a copy of a container of reference_wrappers to be
    // an INDEPENDENT copy of the data. Copying the vector copies the WRAPPERS --
    // both vectors refer to the same underlying objects (x, y, z).
    std::vector<std::reference_wrapper<int>> copy = original;

    // Mutating through the "copy" changes the originals, which the first vector
    // sees too -- they share referents.
    copy[0].get() = 100;
    std::cout << "x = " << x << "\n";                            // 100
    std::cout << "original[0] = " << original[0].get() << "\n";  // 100 -- shared

    // FIX: when you need independent data, copy the VALUES into a container of T.
    std::vector<int> values{original[0].get(), original[1].get(), original[2].get()};
    values[0] = 7;   // independent of x now
    std::cout << "values[0] = " << values[0] << ", x stays " << x << "\n";   // 7, 100
    return 0;
}
