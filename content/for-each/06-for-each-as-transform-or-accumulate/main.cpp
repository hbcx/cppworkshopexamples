#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{2, 4, 6, 8};

    // BAD: for_each with a captured accumulator to compute a sum. It works, but a
    // reader must trace the capture to see that it is just a total.
    long long sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });

    // BAD: for_each pushing mapped values into another vector -- this is a map,
    // dressed up as a side effect.
    std::vector<int> squaredBad;
    std::for_each(v.begin(), v.end(),
                  [&squaredBad](int x) { squaredBad.push_back(x * x); });

    // GOOD: accumulate says "fold to a total"; transform says "map each element".
    long long sum2 = std::accumulate(v.begin(), v.end(), 0LL);
    std::vector<int> squared;
    std::transform(v.begin(), v.end(), std::back_inserter(squared),
                   [](int x) { return x * x; });

    std::cout << "sum (for_each): " << sum << ", sum (accumulate): " << sum2 << '\n';
    std::cout << "squared: ";
    for (int x : squared) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
