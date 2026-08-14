#include <atomic>
#include <execution>
#include <iostream>
#include <mutex>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> data(1000);
    for (int i = 0; i < 1000; ++i) data[i] = i + 1;

    // ANTI-PATTERN: synchronize inside a par_unseq callable.
    //
    //   std::mutex m;
    //   long long sum = 0;
    //   std::for_each(std::execution::par_unseq, data.begin(), data.end(),
    //                 [&](int x){ std::lock_guard<std::mutex> lk(m); sum += x; });
    //
    // par_unseq permits VECTORIZATION: steps from different elements may be
    // interleaved WITHIN one thread. Acquiring a lock (or any blocking sync) in
    // that state is undefined behaviour -- a thread can try to lock a mutex it
    // already holds mid-vector, deadlocking or corrupting it. par_unseq callables
    // must be free of locks, and of anything that blocks or synchronizes. We do
    // not run that version.

    // FIX A: if you need shared state, use par (threads only), where per-element
    // locking or atomics are allowed. An atomic keeps it lock-free.
    std::atomic<long long> sum{0};
    std::for_each(std::execution::par, data.begin(), data.end(),
                  [&](int x) { sum += x; });
    std::cout << "sum via par + atomic: " << sum.load() << '\n';

    // FIX B (best): keep par_unseq but touch NO shared state -- express it as a
    // reduction, which needs no synchronization at all and is the most vectorizable.
    long long reduced =
        std::reduce(std::execution::par_unseq, data.begin(), data.end(), 0LL);
    std::cout << "sum via par_unseq reduce: " << reduced << '\n';
    return 0;
}
