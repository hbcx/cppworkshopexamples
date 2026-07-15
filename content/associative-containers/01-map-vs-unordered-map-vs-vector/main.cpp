// Three containers, one job: map a key to a value.
//
//   std::map            balanced tree, O(log n), nodes scattered over the heap
//   std::unordered_map  hash table, O(1) average, still one indirection per node
//   sorted vector       binary search, O(log n), everything in contiguous memory
//
// The complexity column says unordered_map wins and the other two tie. The
// nanosecond column says something more interesting, because a cache miss costs
// about as much as a hundred comparisons -- and the tree pays one per level.

#include "bench/datasets.hpp"
#include "bench/timing.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {

using bench::Key;
using Entry = std::pair<Key, Key>;

std::map<Key, Key> build_map(const std::vector<Key>& keys) {
    std::map<Key, Key> m;
    for (Key k : keys) {
        m.emplace(k, k);
    }
    return m;
}

std::unordered_map<Key, Key> build_umap(const std::vector<Key>& keys) {
    // No reserve() on purpose: this is what the container costs when you use it
    // the way it is usually written, rehashing included. reserve() is a large,
    // separate win and gets its own example.
    std::unordered_map<Key, Key> m;
    for (Key k : keys) {
        m.emplace(k, k);
    }
    return m;
}

std::vector<Entry> build_vec(const std::vector<Key>& keys) {
    std::vector<Entry> v;
    v.reserve(keys.size());
    for (Key k : keys) {
        v.emplace_back(k, k);
    }
    std::sort(v.begin(), v.end(),
              [](const Entry& a, const Entry& b) { return a.first < b.first; });
    return v;
}

// Sum the values found, so the lookups produce something the optimizer must keep.
template <class MapLike>
std::uint64_t probe(const MapLike& m, const std::vector<Key>& queries) {
    std::uint64_t sum = 0;
    for (Key k : queries) {
        const auto it = m.find(k);
        if (it != m.end()) {
            sum += it->second;
        }
    }
    return sum;
}

std::uint64_t probe(const std::vector<Entry>& v, const std::vector<Key>& queries) {
    std::uint64_t sum = 0;
    for (Key k : queries) {
        const auto it = std::lower_bound(
            v.begin(), v.end(), k,
            [](const Entry& e, Key key) { return e.first < key; });
        if (it != v.end() && it->first == k) {
            sum += it->second;
        }
    }
    return sum;
}

struct Row {
    std::size_t n;
    bench::Stats map;
    bench::Stats umap;
    bench::Stats vec;
};

void print_table(const char* what, const std::vector<Row>& rows) {
    std::cout << "\n  " << what << "   (ns per operation, median; min in parentheses)\n"
              << "  ---------------------------------------------------------------------------\n"
              << "          n     std::map    unordered_map   sorted vector    map/unordered\n"
              << "  ---------------------------------------------------------------------------\n";
    std::cout << std::fixed;
    for (const Row& r : rows) {
        const double ratio = r.umap.median_ns > 0.0
                                 ? r.map.median_ns / r.umap.median_ns
                                 : 0.0;
        std::cout << "  " << std::setw(9) << r.n
                  << std::setprecision(1)
                  << std::setw(9) << r.map.median_ns
                  << " (" << std::setw(6) << r.map.min_ns << ")"
                  << std::setw(9) << r.umap.median_ns
                  << " (" << std::setw(6) << r.umap.min_ns << ")"
                  << std::setw(9) << r.vec.median_ns
                  << " (" << std::setw(6) << r.vec.min_ns << ")"
                  << std::setw(11) << std::setprecision(2) << ratio << "x"
                  << "\n";
    }
}

void print_header() {
    const char* machine = std::getenv("BENCH_MACHINE");
    std::cout << "std::map vs std::unordered_map vs a sorted vector\n"
              << "================================================\n\n";
#if defined(__clang__)
    std::cout << "compiler : clang++ " << __clang_major__ << "." << __clang_minor__
              << "." << __clang_patchlevel__ << "\n";
#elif defined(__GNUC__)
    std::cout << "compiler : g++ " << __GNUC__ << "." << __GNUC_MINOR__
              << "." << __GNUC_PATCHLEVEL__ << "\n";
#else
    std::cout << "compiler : unknown\n";
#endif
    std::cout << "standard : __cplusplus = " << __cplusplus << "\n"
              << "machine  : " << (machine != nullptr ? machine : "unspecified "
                                   "(set BENCH_MACHINE before regenerating)")
              << "\n"
              << "method   : " << bench::kSamples
              << " samples per cell, inner loop calibrated to >= 2 ms per sample\n\n";

#ifndef __OPTIMIZE__
    std::cout << "!! BUILT WITHOUT OPTIMIZATION -- these numbers are meaningless. !!\n"
                 "!! At -O0 you are timing the un-inlined debug shape of the      !!\n"
                 "!! standard library, not the algorithm. Build with -O2.         !!\n\n";
#endif
}

}  // namespace

int main() {
    print_header();

    const std::size_t sizes[] = {100, 1'000, 10'000, 100'000, 1'000'000};
    const bench::Shape shapes[] = {bench::Shape::Random, bench::Shape::Sequential};

    for (bench::Shape shape : shapes) {
        std::vector<Row> build_rows;
        std::vector<Row> hit_rows;
        std::vector<Row> miss_rows;

        for (std::size_t n : sizes) {
            const std::vector<Key> keys = bench::make_keys(shape, n);
            const std::vector<Key> hits = bench::make_hit_queries(keys, n);
            const std::vector<Key> misses = bench::make_miss_queries(keys, n);

            build_rows.push_back(
                Row{n,
                    bench::measure_build(n, [&] { return build_map(keys); }),
                    bench::measure_build(n, [&] { return build_umap(keys); }),
                    bench::measure_build(n, [&] { return build_vec(keys); })});

            const auto m = build_map(keys);
            const auto um = build_umap(keys);
            const auto vec = build_vec(keys);

            hit_rows.push_back(
                Row{n,
                    bench::measure(hits.size(), [&] { return probe(m, hits); }),
                    bench::measure(hits.size(), [&] { return probe(um, hits); }),
                    bench::measure(hits.size(), [&] { return probe(vec, hits); })});

            miss_rows.push_back(
                Row{n,
                    bench::measure(misses.size(), [&] { return probe(m, misses); }),
                    bench::measure(misses.size(), [&] { return probe(um, misses); }),
                    bench::measure(misses.size(), [&] { return probe(vec, misses); })});
        }

        std::cout << "\ndataset: " << bench::shape_name(shape) << "  --  "
                  << bench::shape_note(shape) << "\n";
        print_table("build (insert every key)", build_rows);
        print_table("lookup, key present", hit_rows);
        print_table("lookup, key absent", miss_rows);
        std::cout << "\n";
    }

    return 0;
}
