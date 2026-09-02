#include <limits>
#include <iostream>
#include <vector>

int main() {
    std::vector<double> data{-5.0, -2.0, -9.0, -1.0};   // all negative

    // TRAP: seed a running maximum with numeric_limits<double>::min(), expecting
    // "the most negative double". But for floating types min() is the smallest
    // POSITIVE normal value (~2.2e-308), so every negative element is below it
    // and the seed wins -- the "maximum" is a tiny positive number not in the data.
    double bad = std::numeric_limits<double>::min();
    for (double x : data) if (x > bad) bad = x;
    std::cout << "seeded with min()    -> " << bad << "  (wrong: not from the data)\n";

    // FIX: lowest() is the most negative finite value, for integer and floating
    // types alike.
    double good = std::numeric_limits<double>::lowest();
    for (double x : data) if (x > good) good = x;
    std::cout << "seeded with lowest() -> " << good << "  (correct: -1)\n";
}
