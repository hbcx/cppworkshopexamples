#include <execution>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) data[i] = i + 1;   // 1..1000

    // std::reduce is the parallel-friendly sum: unlike std::accumulate it may
    // combine elements in ANY order and grouping, which is what lets par split
    // the range across threads and add the partial results. So the operation must
    // be associative and commutative (plain + is). Sum of 1..1000 = 500500.
    long long total = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
    std::cout << "parallel sum: " << total << '\n';

    // std::transform_reduce fuses a map and a reduce, so a dot product or a sum of
    // a projected value parallelizes in one pass. Here: sum of squares of 1..1000.
    long long sumSquares = std::transform_reduce(
        std::execution::par, data.begin(), data.end(), 0LL,
        std::plus<long long>{}, [](int x) { return static_cast<long long>(x) * x; });
    std::cout << "parallel sum of squares: " << sumSquares << '\n';

    // Because the result is order-independent, par gives the SAME answer as seq --
    // parallelism is safe here precisely because + is associative. An operation
    // that is not (subtraction, or float sums where rounding depends on order)
    // would give a different, unstable result under par.
    std::cout << "matches sequential? " << std::boolalpha
              << (total == std::reduce(std::execution::seq, data.begin(), data.end(), 0LL))
              << '\n';
    return 0;
}
