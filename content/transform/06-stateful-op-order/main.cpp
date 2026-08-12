#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main() {
    std::vector<int> in{10, 20, 30, 40};

    // BAD: using transform's function to carry STATE -- here a running total -- and
    // relying on it being applied left to right. transform does NOT promise the
    // ORDER in which it applies the function, so a stateful op has an unspecified
    // result. We compute it but do NOT print it: it is not guaranteed, which is the
    // whole point.
    int running = 0;
    std::vector<int> prefixBad(in.size());
    std::transform(in.begin(), in.end(), prefixBad.begin(), [&running](int x){
        running += x;
        return running;   // wants 10, 30, 60, 100 -- but order is not promised
    });

    // FIX: a running total is a SCAN, not a map. std::partial_sum is built for it
    // and is defined to run left to right.
    std::vector<int> prefix(in.size());
    std::partial_sum(in.begin(), in.end(), prefix.begin());
    std::cout << "partial_sum (defined order): ";
    for (int x : prefix) std::cout << x << ' ';
    std::cout << '\n';   // 10 30 60 100

    // The rule: transform's function should be PURE -- depend only on its argument.
    // For per-element work that must happen in order, use for_each or a scan.
    return 0;
}
