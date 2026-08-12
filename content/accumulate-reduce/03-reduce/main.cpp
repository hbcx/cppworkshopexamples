#include <numeric>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> nums{3, 1, 4, 1, 5, 9, 2, 6};

    // std::reduce (C++17) is like accumulate -- a fold with an init and a binary op
    // -- but it does NOT promise an ORDER of combination. That freedom is what lets
    // an implementation split the work across threads. For + it gives the same sum.
    int sum = std::reduce(nums.begin(), nums.end(), 0);
    std::cout << "reduce sum:     " << sum << '\n';   // 31

    int product = std::reduce(nums.begin(), nums.end(), 1, std::multiplies<int>{});
    std::cout << "reduce product: " << product << '\n';   // 6480

    // Because the order is unspecified, the operation MUST be associative and
    // commutative for the result to be well-defined -- + and * are, so these are
    // fine. Subtraction is not, and must never be used with reduce (see the
    // anti-pattern); use accumulate for order-dependent folds. With an execution
    // policy, std::reduce(std::execution::par, ...) runs the same fold in parallel.
    return 0;
}
