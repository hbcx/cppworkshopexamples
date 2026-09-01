#include <random>
#include <iostream>

int main() {
    // The two-part model of <random>:
    //   1) an engine (a URBG) produces raw pseudo-random bits from a seed;
    //   2) a distribution turns those bits into numbers with the shape you want.
    // Here: the Mersenne Twister engine, seeded with a fixed value so the run is
    // reproducible, feeding a uniform distribution over the closed range [1, 6].
    std::mt19937 engine(42);
    std::uniform_int_distribution<int> die(1, 6);

    std::cout << "ten die rolls:";
    for (int i = 0; i < 10; ++i) std::cout << ' ' << die(engine);
    std::cout << "\n";

    // The distribution is a function object: call it with the engine each time.
    std::cout << "one more roll: " << die(engine) << "\n";
}
