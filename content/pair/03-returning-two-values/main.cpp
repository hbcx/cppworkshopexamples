#include <iostream>
#include <utility>
#include <vector>

// Return two related results at once instead of using an out-parameter.
std::pair<int, int> divmod(int a, int b) {
    return std::make_pair(a / b, a % b);
}

// A second common shape: minimum and maximum in a single pass.
std::pair<int, int> minmax(const std::vector<int>& v) {
    int lo = v.front();
    int hi = v.front();
    for (int x : v) {
        if (x < lo) lo = x;
        if (x > hi) hi = x;
    }
    return {lo, hi};   // braced return: the pair type comes from the signature
}

int main() {
    std::pair<int, int> qr = divmod(17, 5);
    std::cout << "17 / 5 = " << qr.first << " rem " << qr.second << '\n';

    std::vector<int> data{4, 9, 1, 7, 2};
    std::pair<int, int> mm = minmax(data);
    std::cout << "min = " << mm.first << ", max = " << mm.second << '\n';
    return 0;
}
