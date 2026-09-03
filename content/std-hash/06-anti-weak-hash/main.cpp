#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

struct Coord {
    int x;
    int y;
};
bool operator==(const Coord& a, const Coord& b) { return a.x == b.x && a.y == b.y; }

// ANTI-PATTERN: a weak hash. Adding the fields collapses many distinct coords to
// the same value -- every point on the line x + y == 10 hashes to 10. An
// unordered_set then piles them all into one bucket and degrades to a linear scan.
struct WeakHash {
    std::size_t operator()(const Coord& c) const {
        return static_cast<std::size_t>(c.x + c.y);   // symmetric AND low-entropy
    }
};

// FIX: a proper mix (hash_combine style) spreads distinct coords across buckets.
struct GoodHash {
    std::size_t operator()(const Coord& c) const {
        std::size_t s = std::hash<int>{}(c.x);
        s ^= std::hash<int>{}(c.y) + 0x9e3779b97f4a7c15ULL + (s << 6) + (s >> 2);
        return s;
    }
};

template <typename Set>
std::size_t maxBucketSize(const Set& s) {
    std::size_t worst = 0;
    for (std::size_t b = 0; b < s.bucket_count(); ++b)
        worst = std::max(worst, s.bucket_size(b));
    return worst;
}

int main() {
    // A diagonal of DISTINCT points, all with x + y == 10.
    std::vector<Coord> pts;
    for (int x = 0; x <= 10; ++x) pts.push_back({x, 10 - x});

    std::unordered_set<Coord, WeakHash> weak(pts.begin(), pts.end());
    std::unordered_set<Coord, GoodHash> good(pts.begin(), pts.end());

    // Both hold all 11 distinct points, but the weak hash puts every one in a
    // single bucket (worst bucket = 11), turning O(1) lookup into O(n).
    std::cout << "weak: " << weak.size() << " elems, worst bucket = "
              << maxBucketSize(weak) << "\n";
    std::cout << "good: " << good.size() << " elems, worst bucket = "
              << maxBucketSize(good) << "\n";
    return 0;
}
