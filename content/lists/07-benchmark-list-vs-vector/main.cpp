#include "bench/datasets.hpp"
#include "bench/timing.hpp"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>

namespace {

using bench::Key;

void header(const char* title, const char* units) {
    std::cout << "\n  " << title << "   (" << units << ")\n";
    std::cout << "  ------------------------------------------------------------\n";
    std::cout << "          n      std::vector       std::list        list/vector\n";
    std::cout << "  ------------------------------------------------------------\n";
}

void row(std::size_t n, bench::Stats vec, bench::Stats lst) {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << std::setw(11) << n
              << std::setw(10) << vec.median_ns << " (" << std::setw(6) << vec.min_ns << ")"
              << std::setw(10) << lst.median_ns << " (" << std::setw(6) << lst.min_ns << ")"
              << std::setw(12) << std::setprecision(2)
              << (vec.median_ns > 0.0 ? lst.median_ns / vec.median_ns : 0.0) << "x\n";
}

// Append every key. No shifting, no searching: just allocation and layout.
void bench_build(const std::vector<std::size_t>& sizes) {
    header("build (push_back every key)", "ns per operation, median; min in parentheses");
    for (std::size_t n : sizes) {
        const auto keys = bench::make_keys(bench::Shape::Random, n);

        const auto vec = bench::measure_build(n, [&] {
            std::vector<Key> v;               // deliberately no reserve(): the
            for (Key k : keys) {              // list cannot preallocate either
                v.push_back(k);
            }
            return v;
        });
        const auto lst = bench::measure_build(n, [&] {
            std::list<Key> l;
            for (Key k : keys) {
                l.push_back(k);
            }
            return l;
        });
        row(n, vec, lst);
    }
}

// Walk the whole container and add everything up. Identical work, identical
// order -- the only difference is where the next element is.
void bench_traverse(const std::vector<std::size_t>& sizes) {
    header("traverse (sum every element)", "ns per element, median; min in parentheses");
    for (std::size_t n : sizes) {
        const auto keys = bench::make_keys(bench::Shape::Random, n);
        const std::vector<Key> v(keys.begin(), keys.end());
        const std::list<Key> l(keys.begin(), keys.end());

        const auto vec = bench::measure(n, [&] {
            std::uint64_t sum = 0;
            for (Key k : v) {
                sum += k;
            }
            return sum;
        });
        const auto lst = bench::measure(n, [&] {
            std::uint64_t sum = 0;
            for (Key k : l) {
                sum += k;
            }
            return sum;
        });
        row(n, vec, lst);
    }
}

// The workload the list is supposed to win: keep a sorted sequence by inserting
// each new key at its place. The vector must shift the tail every single time;
// the list only relinks two pointers -- after it has walked to the position.
void bench_sorted_insert(const std::vector<std::size_t>& sizes) {
    header("sorted insert (find the position, then insert)",
           "ns per inserted element, median; min in parentheses");
    for (std::size_t n : sizes) {
        const auto keys = bench::make_keys(bench::Shape::Random, n);

        const auto vec = bench::measure_build(n, [&] {
            std::vector<Key> v;
            for (Key k : keys) {
                // A linear scan, not lower_bound: the list has no way to binary
                // search, so both containers must do the same search to compare.
                auto pos = std::find_if(v.begin(), v.end(),
                                        [k](Key x) { return x > k; });
                v.insert(pos, k);   // O(n): shifts the tail
            }
            return v;
        });
        const auto lst = bench::measure_build(n, [&] {
            std::list<Key> l;
            for (Key k : keys) {
                auto pos = std::find_if(l.begin(), l.end(),
                                        [k](Key x) { return x > k; });
                l.insert(pos, k);   // O(1): relinks two pointers
            }
            return l;
        });
        row(n, vec, lst);
    }
}

}  // namespace

int main() {
    std::cout << "std::list vs std::vector\n";
    std::cout << "========================\n\n";
#if defined(__clang__)
    std::cout << "compiler : clang++ " << __clang_major__ << "." << __clang_minor__
              << "." << __clang_patchlevel__ << "\n";
#elif defined(__GNUC__)
    std::cout << "compiler : g++ " << __GNUC__ << "." << __GNUC_MINOR__
              << "." << __GNUC_PATCHLEVEL__ << "\n";
#else
    std::cout << "compiler : unknown\n";
#endif
    std::cout << "standard : __cplusplus = " << __cplusplus << '\n';
    std::cout << "machine  : Intel Core i9-13900K, Windows 11, MSYS2 UCRT64\n";
    std::cout << "method   : " << bench::kSamples
              << " samples per cell, inner loop calibrated to >= 2 ms per sample\n";

    bench_build({100, 1000, 10000, 100000, 1000000});
    bench_traverse({100, 1000, 10000, 100000, 1000000});
    // Quadratic in n: 100k would be 10^10 comparisons. The trend is settled well
    // before that.
    bench_sorted_insert({100, 1000, 10000});
    return 0;
}
