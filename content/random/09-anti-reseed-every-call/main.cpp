#include <random>
#include <iostream>

// TRAP: build a fresh engine (seeded the same way) on every call. Each call
// restarts the same sequence, so you get the SAME "random" number every time.
// In real code the seed is often the clock, and all calls within one second
// still collide.
int bad_random() {
    std::mt19937 gen(42);                        // reseeded on every call!
    std::uniform_int_distribution<int> d(1, 100);
    return d(gen);
}

int main() {
    std::cout << "reseeding every call (all identical):";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << bad_random();
    std::cout << "\n";

    // FIX: create the engine ONCE and reuse it, so the sequence advances.
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> d(1, 100);
    std::cout << "one reused engine (advances):";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << d(gen);
    std::cout << "\n";
}
