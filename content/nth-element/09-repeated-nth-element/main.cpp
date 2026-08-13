#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data{50, 12, 88, 7, 33, 91, 4, 60, 21, 77};
    const std::size_t n = data.size();

    // Want several order statistics: the 25th, 50th, and 75th percentiles.
    // BAD: a separate nth_element for each. Each call is O(n) AND reorders the whole
    // range, so three of them cost 3 * O(n) with the data reshuffled each time; for
    // many such queries this adds up and the moving pivots are easy to reason about
    // wrongly.
    std::vector<int> bad = data;
    std::nth_element(bad.begin(), bad.begin() + n / 4, bad.end());
    int p25 = bad[n / 4];
    std::nth_element(bad.begin(), bad.begin() + n / 2, bad.end());
    int p50 = bad[n / 2];
    std::nth_element(bad.begin(), bad.begin() + 3 * n / 4, bad.end());
    int p75 = bad[3 * n / 4];

    // GOOD: for several order statistics, sort ONCE (O(n log n)) and index directly --
    // simpler, and cheaper than many O(n) selections once the query count grows.
    std::vector<int> good = data;
    std::sort(good.begin(), good.end());

    std::cout << "percentiles via repeated nth_element: "
              << p25 << ' ' << p50 << ' ' << p75 << '\n';
    std::cout << "percentiles via one sort:             "
              << good[n / 4] << ' ' << good[n / 2] << ' ' << good[3 * n / 4] << '\n';
    return 0;
}
