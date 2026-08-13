#include <numeric>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // partial_sum writes running (prefix) sums: out[i] is the sum of in[0..i], so
    // 1, 1+2, 1+2+3, ... The last element is the total of the whole range.
    std::vector<int> sums;
    std::partial_sum(v.begin(), v.end(), std::back_inserter(sums));
    std::cout << "prefix sums: ";
    for (int x : sums) std::cout << x << ' ';
    std::cout << '\n';

    // It can also write back into the SAME range (in place).
    std::partial_sum(v.begin(), v.end(), v.begin());
    std::cout << "in place:    ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
