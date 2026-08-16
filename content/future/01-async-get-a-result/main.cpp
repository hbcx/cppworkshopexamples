#include <future>
#include <iostream>
#include <numeric>
#include <vector>

// The simplest way to run something on another thread and get its return value
// back. std::async starts the function (here on a new thread, because we ask for
// launch::async) and immediately returns a std::future -- a handle to a result
// that is not ready yet. Calling future.get() blocks until the function has
// finished and then hands you what it returned. No shared variable, no manual
// join, no mutex: the future carries the value and does the synchronization.
long sum_range(int lo, int hi) {
    std::vector<int> v(hi - lo + 1);
    std::iota(v.begin(), v.end(), lo);          // fill lo, lo+1, ..., hi
    return std::accumulate(v.begin(), v.end(), 0L);
}

int main() {
    // Launch sum_range(1, 10) on another thread. We get a future right away and
    // can keep doing other work here while it runs.
    std::future<long> f = std::async(std::launch::async, sum_range, 1, 10);

    // ... other work could happen here ...

    long result = f.get();                      // block until the result is ready
    std::cout << "sum 1..10 = " << result << '\n';   // 55
    return 0;
}
