#include <random>
#include <iostream>
#include <cstdint>

// A tiny reproducible source of randomness for a simulation or game: it owns
// the engine, remembers the seed it was built with (so a run can be replayed),
// and exposes shaped draws. One instance = one reproducible stream.
class RandomSource {
public:
    explicit RandomSource(std::uint32_t seed) : seed_(seed), engine_(seed) {}

    std::uint32_t seed() const { return seed_; }

    int roll(int lo, int hi) {
        std::uniform_int_distribution<int> d(lo, hi);
        return d(engine_);
    }
    double unit() {
        std::uniform_real_distribution<double> d(0.0, 1.0);
        return d(engine_);
    }

private:
    std::uint32_t seed_;
    std::mt19937  engine_;
};

int main() {
    // Pick a seed once (from the OS in real code) and log it, so the exact run
    // can be reproduced later from the logged number.
    RandomSource rng(20240501);
    std::cout << "run seed = " << rng.seed() << "  (log this to replay)\n";

    std::cout << "rolls:";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << rng.roll(1, 100);
    std::cout << "\n";

    // Rebuilding with the SAME seed reproduces the identical stream.
    RandomSource replay(rng.seed());
    std::cout << "replay:";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << replay.roll(1, 100);
    std::cout << "\n";
}
