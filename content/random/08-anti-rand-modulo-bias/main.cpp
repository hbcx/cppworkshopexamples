#include <random>
#include <iostream>
#include <array>

int main() {
    // TRAP: the classic "rand() % n" (or "engine() % n") folds the generator's
    // output range into n buckets with a remainder. When the range size is not
    // a multiple of n, the first few buckets get one extra value each -- a
    // modulo BIAS toward the small numbers. We show it with pure arithmetic:
    // fold a generator range of size 100 into 6 buckets (like a die).
    const int M = 100;   // pretend the generator yields 0..99
    const int n = 6;
    std::array<int, 6> biased{};
    for (int v = 0; v < M; ++v) ++biased[v % n];   // exactly what "% n" does
    std::cout << "folding 0..99 with % 6 (uneven counts = bias):";
    for (int c : biased) std::cout << ' ' << c;    // 17 17 17 17 16 16
    std::cout << "\n";

    // FIX: std::uniform_int_distribution maps without bias -- it rejects the
    // few leftover values so every outcome is equally likely. No global state,
    // and the engine is passed explicitly.
    std::mt19937 gen(1);
    std::uniform_int_distribution<int> die(0, 5);
    std::array<int, 6> fair{};
    for (int i = 0; i < 60000; ++i) ++fair[die(gen)];
    std::cout << "uniform_int_distribution over 60000 draws (even):";
    for (int c : fair) std::cout << ' ' << c;
    std::cout << "\n";
}
