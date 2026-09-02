#include <numeric>
#include <utility>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    // Several range-sum queries [lo, hi).
    std::vector<std::pair<int, int>> queries{{0, 4}, {2, 6}, {1, 8}, {3, 5}};

    // BAD: summing each query's range from scratch. Each query scans its range --
    // O(n) per query, O(n*q) for q queries -- redoing overlapping work every time.
    std::cout << "per-query scan:  ";
    for (auto q : queries) {
        int s = std::accumulate(v.begin() + q.first, v.begin() + q.second, 0);
        std::cout << s << ' ';
    }
    std::cout << '\n';

    // GOOD: precompute a prefix-sum array ONCE (O(n)), with prefix[0] = 0. Then each
    // range sum [lo, hi) is a single subtraction prefix[hi] - prefix[lo] -- O(1).
    std::vector<int> prefix(v.size() + 1, 0);
    std::partial_sum(v.begin(), v.end(), prefix.begin() + 1);
    std::cout << "prefix-sum O(1): ";
    for (auto q : queries)
        std::cout << (prefix[q.second] - prefix[q.first]) << ' ';
    std::cout << '\n';
    return 0;
}
