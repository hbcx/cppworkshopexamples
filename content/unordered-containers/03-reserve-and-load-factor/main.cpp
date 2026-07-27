// A hash table keeps its buckets roughly as numerous as its elements: the load
// factor (size / bucket_count) is held under max_load_factor (1.0 by default).
// When an insert would push it over, the table REHASHES -- allocates a bigger
// bucket array and re-files every element. Growing from empty therefore rehashes
// several times. reserve(n) pre-sizes the bucket array so a known bulk insert
// never rehashes at all.
//
// The exact bucket counts are implementation-defined, so this example reports
// only portable facts: whether a rehash happened, and whether the load factor
// stayed under its limit.

#include <cstddef>
#include <iostream>
#include <unordered_map>

namespace {

// Fill a map with n entries and report whether bucket_count() ever changed while
// filling -- i.e. whether the table had to rehash.
bool rehashed_while_filling(bool doReserve, int n) {
    std::unordered_map<int, int> m;
    if (doReserve) {
        m.reserve(static_cast<std::size_t>(n));
    }
    const std::size_t startBuckets = m.bucket_count();
    for (int i = 0; i < n; ++i) {
        m[i] = i;
    }
    return m.bucket_count() != startBuckets;
}

}  // namespace

int main() {
    const int n = 10000;

    std::cout << "inserting " << n << " entries into an unordered_map\n";
    std::cout << "  without reserve: rehashed while filling? "
              << (rehashed_while_filling(false, n) ? "yes" : "no") << "\n";
    std::cout << "  with reserve(" << n << "): rehashed while filling? "
              << (rehashed_while_filling(true, n) ? "yes" : "no") << "\n";

    // After reserve + fill the invariant still holds: load never crossed the max.
    std::unordered_map<int, int> m;
    m.reserve(static_cast<std::size_t>(n));
    for (int i = 0; i < n; ++i) {
        m[i] = i;
    }
    std::cout << "after reserve+fill: load_factor <= max_load_factor? "
              << (m.load_factor() <= m.max_load_factor() ? "yes" : "no") << "\n";
    return 0;
}
