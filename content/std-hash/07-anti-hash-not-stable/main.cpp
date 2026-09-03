#include <cstdint>
#include <iostream>
#include <string>

// A defined, stable hash (FNV-1a, 64-bit) for when you need a value that is the
// SAME across runs, machines and compilers -- because std::hash does not promise
// that.
std::uint64_t fnv1a(const std::string& s) {
    std::uint64_t h = 1469598103934665603ULL;      // FNV-1a offset basis
    for (unsigned char c : s) {
        h ^= c;
        h *= 1099511628211ULL;                      // FNV-1a prime
    }
    return h;
}

int main() {
    // ANTI-PATTERN: storing or transmitting a std::hash value as if it were a
    // stable identifier or checksum. The standard makes NO guarantee that
    // std::hash returns the same result across program runs, standard-library
    // implementations, or platforms -- and some libraries deliberately randomize
    // string hashing per process to resist collision attacks. A value keyed by
    // std::hash today may not match tomorrow's run or another machine.
    std::cout << "std::hash is fine INSIDE one run (e.g. for unordered_map),\n"
                 "but do not persist it: no cross-run or cross-build promise.\n";

    // FIX: for a persistent id, a checksum, or an on-disk / network key, use a
    // DEFINED algorithm whose output is specified -- here FNV-1a, which yields the
    // same number everywhere.
    std::cout << "fnv1a(\"cppworkshop\") = " << fnv1a("cppworkshop") << "\n";
    std::cout << "fnv1a(\"cppworkshop\") = " << fnv1a("cppworkshop") << "\n";  // identical, and reproducible elsewhere
    return 0;
}
