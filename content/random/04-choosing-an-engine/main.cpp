#include <random>
#include <iostream>

// Seed an engine, discard a warm-up run, then return its next raw value. Raw
// engine output is standardized per engine, so this is reproducible.
template <class Engine>
unsigned long long value_after_warmup(unsigned seed) {
    Engine e(seed);
    for (int i = 0; i < 1000; ++i) e();   // discard some output
    return e();
}

int main() {
    // The standard engines trade quality, speed and state size. A few common
    // ones, each seeded the same way:
    const unsigned seed = 2024;

    std::cout << "minstd_rand (tiny, fast, low quality): "
              << value_after_warmup<std::minstd_rand>(seed) << "\n";
    std::cout << "mt19937     (usual default, big state): "
              << value_after_warmup<std::mt19937>(seed) << "\n";
    std::cout << "mt19937_64  (64-bit Mersenne Twister):  "
              << value_after_warmup<std::mt19937_64>(seed) << "\n";
    std::cout << "ranlux24    (slow, very high quality):  "
              << value_after_warmup<std::ranlux24>(seed) << "\n";

    std::cout << "state sizes: mt19937=" << sizeof(std::mt19937)
              << " bytes, minstd_rand=" << sizeof(std::minstd_rand) << " bytes\n";

    // Rule of thumb: mt19937 for general use; minstd for cheap throwaway noise;
    // a ranlux-class engine only when quality matters -- none for cryptography.
}
