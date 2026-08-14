#include <iostream>
#include <random>
#include <vector>

int main() {
    // ANTI-PATTERN: int d = rand() % 6 + 1;
    //
    // Two problems. std::rand is a low-quality generator, and "% n" adds MODULO
    // BIAS: unless the number of raw values divides evenly by n, some buckets
    // get one more raw value than others, so those outcomes are more likely.
    //
    // We show the bias with pure counting -- no rand() here, because its range
    // (RAND_MAX) differs between platforms. RANGE stands in for a typical raw
    // range; map every raw value 0..RANGE-1 to a bucket with "% 6" and count.
    const int RANGE = 32768;   // e.g. a 15-bit generator: not a multiple of 6
    const int n = 6;
    std::vector<int> buckets(n, 0);
    for (int raw = 0; raw < RANGE; ++raw) ++buckets[raw % n];

    std::cout << "raw values per bucket with % 6:";
    for (int c : buckets) std::cout << ' ' << c;
    std::cout << "  <- not all equal = bias\n";

    // FIX: a proper engine plus std::uniform_int_distribution, which handles the
    // leftover values correctly (it re-draws them) so every outcome is equally
    // likely -- no modulo bias, and a far better generator than rand().
    std::mt19937 engine(555);
    std::uniform_int_distribution<int> dice(1, 6);
    std::cout << "unbiased dice:";
    for (int i = 0; i < 8; ++i) std::cout << ' ' << dice(engine);
    std::cout << '\n';
    return 0;
}
