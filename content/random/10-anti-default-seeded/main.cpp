#include <random>
#include <iostream>

int main() {
    // TRAP: a default-constructed engine uses a FIXED default seed (5489 for
    // mt19937), so it produces the exact same sequence on every program run.
    // A fresh process does NOT give fresh numbers until you seed it.
    std::mt19937 def;                                  // always the same seed
    std::uniform_int_distribution<int> d(1, 100);
    std::cout << "default-seeded (identical on every run):";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << d(def);
    std::cout << "\n";

    // FIX: seed from the OS at startup for run-to-run variety:
    //     std::random_device rd; std::mt19937 gen(rd());
    // We print a fixed-seed stream here so the example itself stays
    // reproducible, but the point is that an EXPLICIT seed is required.
    std::mt19937 gen(1234);
    std::cout << "explicitly seeded:";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << d(gen);
    std::cout << "\n";
}
