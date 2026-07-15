#pragma once

// Test datasets shared by the benchmark examples.
//
// Data is GENERATED from a fixed seed, never committed: a million keys in git
// would be noise, and a reader who can see the generator can decide for himself
// whether the numbers mean anything.
//
// Determinism across standard libraries is the whole point of the hand-written
// Rng below. std::mt19937 is specified bit-for-bit by the standard, so the same
// seed gives the same 32-bit stream everywhere -- but std::uniform_int_distribution
// and std::shuffle are NOT specified: how they turn that stream into a bounded
// value is implementation-defined, and libstdc++ and libc++ genuinely differ.
// Feeding g++ and clang++ different data and then comparing their timings would
// be measuring the data, not the compilers, so we do the reduction and the
// shuffle ourselves.

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

namespace bench {

using Key = std::uint32_t;

inline constexpr std::uint32_t kDefaultSeed = 20260714u;

class Rng {
public:
    explicit Rng(std::uint32_t seed) : eng_(seed) {}

    std::uint32_t next() { return eng_(); }

    // Lemire's multiply-shift reduction into [0, bound). The bias is on the
    // order of bound / 2^32 -- irrelevant for generating test data, and unlike a
    // distribution object this produces identical values on every implementation.
    std::uint32_t below(std::uint32_t bound) {
        return static_cast<std::uint32_t>(
            (static_cast<std::uint64_t>(next()) * bound) >> 32);
    }

private:
    std::mt19937 eng_;
};

// Fisher-Yates, written out for the same reason std::shuffle is avoided.
template <class T>
void shuffle(std::vector<T>& v, Rng& rng) {
    for (std::size_t i = v.size(); i > 1; --i) {
        const std::size_t j = rng.below(static_cast<std::uint32_t>(i));
        std::swap(v[i - 1], v[j]);
    }
}

// The shapes a key set can take. Which one matters depends on what you measure:
// associative containers care about the LAYOUT of the keys (dense vs scattered),
// sorting algorithms care about the ORDER they arrive in.
enum class Shape {
    Random,        // distinct keys scattered over the whole 32-bit range
    Sequential,    // 0, 2, 4, ... : dense, evenly spaced, already sorted
    Reversed,      // the same, descending: worst case for a naive pivot
    NearlySorted,  // sorted, then every element nudged by at most kDisplacement
    FewUnique,     // only kDistinctValues distinct values, heavily repeated
    BucketAligned, // multiples of 64 -- see the note below
};

// Dense shapes step by 2 rather than 1, which leaves the odd numbers free to
// serve as keys that are ABSENT but still lie inside the key range. With keys
// 0..n-1 there is no such value: every miss query has to come from outside the
// range, and a lookup for "bigger than everything" walks the same rightmost path
// of the tree every single time -- a path that then sits in L1 and reports an
// absurdly fast miss. That is a property of the test, not of std::map.
inline constexpr Key kStride = 2;

inline constexpr std::size_t kDisplacement = 16;
inline constexpr std::uint32_t kDistinctValues = 10;

inline const char* shape_name(Shape s) {
    switch (s) {
        case Shape::Random:        return "random";
        case Shape::Sequential:    return "sequential";
        case Shape::Reversed:      return "reversed";
        case Shape::NearlySorted:  return "nearly sorted";
        case Shape::FewUnique:     return "few unique";
        case Shape::BucketAligned: return "bucket aligned";
    }
    return "?";
}

inline const char* shape_note(Shape s) {
    switch (s) {
        case Shape::Random:
            return "distinct keys scattered over the 32-bit range";
        case Shape::Sequential:
            return "dense, evenly spaced, already sorted -- the cache-friendly case";
        case Shape::Reversed:
            return "the same keys, descending: sorted the wrong way";
        case Shape::NearlySorted:
            return "sorted, then each element displaced by at most 16 positions";
        case Shape::FewUnique:
            return "only 10 distinct values -- an associative container ends up tiny";
        case Shape::BucketAligned:
            // Worth knowing, and worth not overselling: libstdc++ and libc++ pick
            // a PRIME bucket count, so hash % buckets scatters multiples of 64
            // just fine. This shape is a disaster only where the bucket count is
            // a power of two (many third-party hash maps), because std::hash for
            // an integer is the identity -- the low bits ARE the bucket index.
            return "multiples of 64 -- pathological for power-of-two bucket counts";
    }
    return "";
}

// n keys of the requested shape. Random/Sequential/Reversed/NearlySorted and
// BucketAligned produce distinct keys; FewUnique deliberately does not.
inline std::vector<Key> make_keys(Shape shape, std::size_t n,
                                  std::uint32_t seed = kDefaultSeed) {
    Rng rng(seed);
    std::vector<Key> keys;
    keys.reserve(n);

    switch (shape) {
        case Shape::Random: {
            // Rejection against a set: with n well under 2^32 the retry rate is
            // negligible, and we get genuinely unstructured DISTINCT keys -- a
            // multiplicative permutation of 0..n-1 would be distinct too, but it
            // lays the keys on a lattice, which is exactly the kind of hidden
            // structure a hash benchmark must not smuggle in.
            std::unordered_set<Key> seen;
            seen.reserve(n * 2);
            while (keys.size() < n) {
                const Key k = rng.next();
                if (seen.insert(k).second) {
                    keys.push_back(k);
                }
            }
            break;
        }
        case Shape::Sequential:
            for (std::size_t i = 0; i < n; ++i) {
                keys.push_back(static_cast<Key>(i) * kStride);
            }
            break;

        case Shape::Reversed:
            for (std::size_t i = n; i > 0; --i) {
                keys.push_back(static_cast<Key>(i - 1) * kStride);
            }
            break;

        case Shape::NearlySorted: {
            for (std::size_t i = 0; i < n; ++i) {
                keys.push_back(static_cast<Key>(i) * kStride);
            }
            for (std::size_t i = 0; i + 1 < n; ++i) {
                const std::size_t span =
                    std::min(kDisplacement, n - 1 - i);
                if (span == 0) {
                    continue;
                }
                const std::size_t j =
                    i + rng.below(static_cast<std::uint32_t>(span + 1));
                std::swap(keys[i], keys[j]);
            }
            break;
        }
        case Shape::FewUnique:
            for (std::size_t i = 0; i < n; ++i) {
                keys.push_back(static_cast<Key>(rng.below(kDistinctValues)));
            }
            break;

        case Shape::BucketAligned:
            for (std::size_t i = 0; i < n; ++i) {
                keys.push_back(static_cast<Key>(i) * 64u);
            }
            break;
    }
    return keys;
}

// m queries that all HIT: drawn from the key set and shuffled.
//
// The shuffle is not cosmetic. Querying keys in ascending order walks a sorted
// vector and a std::map front to back, which the prefetcher loves -- it flatters
// both and quietly hides the random-access cost that real lookup traffic pays.
inline std::vector<Key> make_hit_queries(const std::vector<Key>& keys, std::size_t m,
                                         std::uint32_t seed = kDefaultSeed + 1) {
    Rng rng(seed);
    std::vector<Key> q;
    q.reserve(m);
    for (std::size_t i = 0; i < m; ++i) {
        q.push_back(keys[i % keys.size()]);
    }
    shuffle(q, rng);
    return q;
}

// m queries that all MISS. This is where a hash lookup is at its strongest and
// an ordered lookup at its weakest: a miss still costs std::map a full descent
// to a leaf, while unordered_map usually rejects it after one bucket probe.
//
// The candidates are drawn from INSIDE [min key, max key] on purpose. Drawing
// them from the whole 32-bit range would, for a dense key set, produce queries
// that are all larger than every key -- and searching for "bigger than
// everything" retraces one short path through the container over and over until
// it is pinned in L1. The result is a miss that looks 20x faster than it is.
inline std::vector<Key> make_miss_queries(const std::vector<Key>& keys, std::size_t m,
                                          std::uint32_t seed = kDefaultSeed + 2) {
    Rng rng(seed);
    const std::unordered_set<Key> present(keys.begin(), keys.end());
    const auto range = std::minmax_element(keys.begin(), keys.end());
    const Key lo = *range.first;
    const Key hi = *range.second;
    const std::uint64_t span = static_cast<std::uint64_t>(hi) - lo + 1;

    std::vector<Key> q;
    q.reserve(m);
    Key overflow = hi;  // fallback, should the range hold no free value at all
    while (q.size() < m) {
        bool placed = false;
        for (int attempt = 0; attempt < 64 && !placed; ++attempt) {
            const Key k = lo + (span >= (std::uint64_t{1} << 32)
                                    ? rng.next()
                                    : rng.below(static_cast<std::uint32_t>(span)));
            if (present.find(k) == present.end()) {
                q.push_back(k);
                placed = true;
            }
        }
        if (!placed) {
            q.push_back(++overflow);  // a fully dense range: nothing free inside
        }
    }
    shuffle(q, rng);
    return q;
}

}  // namespace bench
