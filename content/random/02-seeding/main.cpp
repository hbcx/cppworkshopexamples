#include <random>
#include <iostream>

int main() {
    // A fixed seed makes the whole sequence reproducible -- essential for
    // debugging, tests and simulations you must replay exactly.
    std::mt19937 a(12345);
    std::mt19937 b(12345);
    std::uniform_int_distribution<int> d(0, 999);

    std::cout << "same seed -> same sequence:\n ";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << d(a);
    std::cout << "\n ";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << d(b);
    std::cout << "\n";

    // mt19937 has a huge internal state; a single 32-bit seed reaches only part
    // of it. seed_seq mixes several values across the whole state -- a better
    // seeding when you have more than one number of seed material.
    std::seed_seq seq{1, 2, 3, 4};
    std::mt19937 c(seq);
    std::cout << "seed_seq-seeded:";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << d(c);
    std::cout << "\n";

    // In real code you seed once, at startup, from the OS:
    //     std::random_device rd; std::mt19937 gen(rd());
    // then reuse gen. Its values differ every run, so we do not print them here.
}
