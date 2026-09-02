#include <numeric>
#include <iterator>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // transform_inclusive_scan fuses a per-element transform INTO an inclusive
    // scan: each element is mapped first, then the running sum includes it. Here
    // we map x -> x*x and add, giving running sums of squares in a single pass,
    // with no temporary "squared" vector.
    // Argument order is the thing to remember: the BINARY combine op comes
    // first, the UNARY transform second.
    std::vector<int> out;
    std::transform_inclusive_scan(
        v.begin(), v.end(), std::back_inserter(out),
        std::plus<>{},                 // binary: how to combine two values
        [](int x) { return x * x; });  // unary: how to map each element first

    std::cout << "running sum of squares : ";
    for (int x : out) std::cout << x << ' ';    // 1 5 14 30 55
    std::cout << '\n';

    // There is also an init overload: (..., binary, unary, init). The init is
    // folded into every output, since an inclusive scan includes it throughout.
    std::vector<int> out2;
    std::transform_inclusive_scan(
        v.begin(), v.end(), std::back_inserter(out2),
        std::plus<>{}, [](int x) { return x * x; }, 100);

    std::cout << "same, but started at 100: ";
    for (int x : out2) std::cout << x << ' ';   // 101 105 114 130 155
    std::cout << '\n';
    return 0;
}
