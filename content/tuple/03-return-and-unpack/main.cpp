#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

// Return three related results at once. Where pair tops out at two, a tuple
// carries as many as the computation produces.
std::tuple<int, int, double> stats(const std::vector<int>& v) {
    int lo = v.front();
    int hi = v.front();
    long sum = 0;
    for (int x : v) {
        lo = std::min(lo, x);
        hi = std::max(hi, x);
        sum += x;
    }
    return {lo, hi, static_cast<double>(sum) / static_cast<double>(v.size())};
}

int main() {
    std::vector<int> data{4, 9, 1, 7, 2};

    // C++17 structured bindings name the three results at the call site -- far
    // clearer than get<0>/get<1>/get<2>.
    auto [lo, hi, mean] = stats(data);
    std::cout << "min=" << lo << " max=" << hi << " mean=" << mean << '\n';
    return 0;
}
