#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

// A stateful functor: it accumulates as it is called.
struct Accumulator {
    long total = 0;
    int calls = 0;
    void operator()(int x) { total += x; ++calls; }
};

int main() {
    std::vector<int> nums{1, 2, 3, 4, 5};

    // std::for_each takes its functor BY VALUE, so it works on a COPY -- your own
    // Accumulator would stay at zero. A std::reference_wrapper<F> is itself
    // callable (it forwards operator() to the referent), so std::ref(acc) passes
    // the functor by reference and the state lands in your object.
    Accumulator acc;
    std::for_each(nums.begin(), nums.end(), std::ref(acc));
    std::cout << "with std::ref -> total = " << acc.total
              << ", calls = " << acc.calls << "\n";   // 15, 5

    // Without std::ref the accumulation happens on for_each's internal copy and
    // is lost when you ignore the returned copy.
    Accumulator lost;
    std::for_each(nums.begin(), nums.end(), lost);
    std::cout << "without std::ref -> lost.total = " << lost.total << "\n";   // 0
    return 0;
}
