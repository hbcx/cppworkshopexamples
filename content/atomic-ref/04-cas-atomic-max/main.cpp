#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Not every update maps to a fetch_ function. To set a shared value to the MAXIMUM
// each thread proposes, you need a compare-exchange (CAS) loop: read the current
// value, compute the candidate, and swap it in only if nobody changed the value
// meanwhile -- retrying if they did. atomic_ref exposes compare_exchange_weak just
// like std::atomic, so the loop runs on a plain int.
int main() {
    int highest = 0;                    // a plain int, updated atomically below

    std::vector<std::jthread> workers;
    for (int t = 1; t <= 8; ++t) {
        workers.emplace_back([&, t] {
            int candidate = t * 10;                  // this thread's value: 10..80
            std::atomic_ref<int> ref{highest};
            int current = ref.load(std::memory_order_relaxed);
            // Raise highest to candidate only while it is still smaller. On failure
            // compare_exchange_weak refreshes current, so we re-test and retry.
            while (candidate > current &&
                   !ref.compare_exchange_weak(current, candidate,
                                              std::memory_order_relaxed)) {
            }
        });
    }
    workers.clear();                    // join all

    std::cout << "highest value seen = " << highest << '\n';   // 80
    return 0;
}
