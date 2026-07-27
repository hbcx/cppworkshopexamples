// Anti-pattern: a hash function that ignores most of the key. The container
// still returns correct answers, so nothing looks broken -- but every key that
// shares the hashed part lands in the same bucket, and a lookup then walks a
// linked list of everything in that bucket. The O(1) average degrades to O(n),
// silently, and only under load.

#include <cstddef>
#include <functional>
#include <iostream>
#include <unordered_map>

namespace {

struct Item {
    int category;
    int id;
};

bool operator==(const Item& a, const Item& b) {
    return a.category == b.category && a.id == b.id;
}

// THE TRAP: hash only `category`. Every item in the same category collides,
// whatever the bucket count, so they all chain into one bucket.
struct WeakHash {
    std::size_t operator()(const Item& it) const {
        return std::hash<int>()(it.category);
    }
};

// THE FIX: mix every field operator== compares, so distinct items spread out.
struct GoodHash {
    std::size_t operator()(const Item& it) const {
        const std::size_t hc = std::hash<int>()(it.category);
        const std::size_t hi = std::hash<int>()(it.id);
        return hc ^ (hi * 0x9e3779b9u + (hc << 6) + (hc >> 2));
    }
};

template <class Hash>
std::size_t largest_bucket(const std::unordered_map<Item, int, Hash>& m) {
    std::size_t worst = 0;
    for (std::size_t b = 0; b < m.bucket_count(); ++b) {
        if (m.bucket_size(b) > worst) {
            worst = m.bucket_size(b);
        }
    }
    return worst;
}

}  // namespace

int main() {
    const int n = 1000;

    // Identical keys go into both maps: one category, ids 0..n-1.
    std::unordered_map<Item, int, WeakHash> weak;
    std::unordered_map<Item, int, GoodHash> good;
    for (int i = 0; i < n; ++i) {
        weak[Item{0, i}] = i;
        good[Item{0, i}] = i;
    }

    std::cout << "n = " << n << " keys, all sharing category 0\n";

    // With the weak hash all n keys hash equal, so they occupy ONE bucket
    // regardless of how many buckets exist -- deterministically all of them.
    std::cout << "weak hash: largest bucket holds " << largest_bucket(weak)
              << " of " << weak.size() << " keys -> every lookup scans them all\n";

    // With the good hash the keys spread out; the biggest bucket is a tiny
    // fraction of the total. (The exact figure is implementation-defined, so we
    // report the fact that it is small rather than a specific count.)
    const bool spreadOut = largest_bucket(good) * 20 <= static_cast<std::size_t>(n);
    std::cout << "good hash: largest bucket is a small fraction of "
              << good.size() << " keys? " << (spreadOut ? "yes" : "no") << "\n";
    return 0;
}
