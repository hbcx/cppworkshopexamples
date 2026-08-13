#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{4, 8, 15, 16, 23, 42};

    // BAD: computing the whole prefix-sum array just to read its last element for
    // the total. partial_sum allocates and fills n intermediate sums you throw away.
    std::vector<int> sums;
    std::partial_sum(v.begin(), v.end(), std::back_inserter(sums));
    int totalBad = sums.back();

    // GOOD: if you only need the TOTAL, accumulate folds the range to one value --
    // no intermediate array, no allocation.
    int totalGood = std::accumulate(v.begin(), v.end(), 0);

    std::cout << "total (partial_sum.back): " << totalBad << '\n';
    std::cout << "total (accumulate):       " << totalGood << '\n';
    return 0;
}
