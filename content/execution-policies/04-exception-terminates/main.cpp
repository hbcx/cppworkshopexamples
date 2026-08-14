#include <atomic>
#include <execution>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> data{4, 8, 15, 16, -1, 23, 42};   // -1 is a bad value

    // ANTI-PATTERN: throw out of a parallel algorithm's callable to signal an
    // error.
    //
    //   std::for_each(std::execution::par, data.begin(), data.end(),
    //                 [](int x){ if (x < 0) throw std::runtime_error("bad"); ... });
    //
    // If a callable passed to a parallel algorithm throws and the exception would
    // escape the algorithm, the runtime calls std::TERMINATE -- the exception is
    // NOT propagated to the caller, so a try/catch around the call cannot catch
    // it. Throwing to report a bad element aborts the whole program. We do not run
    // that version (it would terminate the process).

    // FIX: report errors WITHOUT throwing. Record the problem in an atomic flag
    // (or count) that every thread can set safely, then check it after the call.
    std::atomic<bool> sawBad{false};
    std::atomic<long long> sum{0};
    std::for_each(std::execution::par, data.begin(), data.end(),
                  [&](int x) {
                      if (x < 0) { sawBad = true; return; }   // skip, flag it
                      sum += x;
                  });

    if (sawBad.load()) {
        std::cout << "found a bad value; sum of the valid ones is " << sum.load()
                  << '\n';
    } else {
        std::cout << "all valid; sum is " << sum.load() << '\n';
    }
    return 0;
}
