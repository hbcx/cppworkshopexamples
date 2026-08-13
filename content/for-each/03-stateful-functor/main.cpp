#include <algorithm>
#include <vector>
#include <iostream>

// A function object that accumulates a running sum and count as it is applied.
struct Stats {
    long long sum = 0;
    int count = 0;
    void operator()(int x) {
        sum += x;
        ++count;
    }
};

int main() {
    std::vector<int> v{4, 8, 15, 16, 23, 42};

    // for_each RETURNS its function object (by value) after applying it to every
    // element, so a stateful functor can gather a result you read back afterward.
    Stats s = std::for_each(v.begin(), v.end(), Stats{});

    std::cout << "count: " << s.count << ", sum: " << s.sum
              << ", average: " << (s.sum / s.count) << '\n';
    return 0;
}
