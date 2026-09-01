#include <random>
#include <iostream>

int main() {
    std::random_device rd;

    // TRAP: std::random_device is MEANT to be a real, non-deterministic source
    // (typically the OS entropy pool), but the standard does not require it.
    // Some implementations -- classic MinGW is the famous case -- returned a
    // FIXED sequence, so seeding from it gave the same "random" numbers on every
    // run. entropy() reports the claimed entropy; 0.0 means "no guarantee". It
    // is a hint, never a promise.
    std::cout << "random_device.entropy() = " << rd.entropy() << "\n";

    // The safe pattern: use random_device ONLY to seed a good engine, and if
    // reproducibility ever appears where you expected randomness, check the
    // platform. We seed an mt19937 and exercise it, but do not print its output
    // (on a working random_device it differs every run).
    std::mt19937 gen(rd());
    (void)gen();
    std::cout << "seeded an mt19937 from random_device (values vary per run)\n";

    // For anything that must be UNPREDICTABLE (tokens, keys, session ids), do
    // not use <random> at all -- reach for the OS cryptographic RNG.
}
