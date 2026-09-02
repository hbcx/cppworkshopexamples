#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> original{10, 12, 11, 15, 14};   // e.g. sensor readings

    // adjacent_difference and partial_sum are INVERSES. Delta-ENCODE by taking
    // differences: the first value stays, the rest become small deltas (which
    // compress well when the data changes slowly).
    std::vector<int> deltas;
    std::adjacent_difference(original.begin(), original.end(),
                             std::back_inserter(deltas));
    std::cout << "deltas:   ";
    for (int x : deltas) std::cout << x << ' ';
    std::cout << '\n';

    // DECODE by taking the prefix sum of the deltas -- it reconstructs the original
    // exactly, because partial_sum undoes adjacent_difference.
    std::vector<int> restored;
    std::partial_sum(deltas.begin(), deltas.end(), std::back_inserter(restored));
    std::cout << "restored: ";
    for (int x : restored) std::cout << x << ' ';
    std::cout << " (matches original: " << std::boolalpha
              << (restored == original) << ")\n";
    return 0;
}
