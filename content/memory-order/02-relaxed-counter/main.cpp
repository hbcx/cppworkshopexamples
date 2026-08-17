#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// memory_order_relaxed guarantees only ATOMICITY -- no ordering relative to any other
// memory operation. That is exactly, and only, enough for a value whose count is never
// used to release or acquire other data: a pure event counter or statistic. The
// increments cannot be lost or torn, though two threads may briefly disagree on the
// running total. Relaxed is the cheapest order; use it when you truly need nothing but
// a correct final count.
int main() {
    std::atomic<long> hits{0};

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k) {
                hits.fetch_add(1, std::memory_order_relaxed);   // atomic, unordered
            }
        });
    }
    workers.clear();                    // join all: a happens-before edge, so the
                                        // final load sees every increment.

    std::cout << "relaxed counter = " << hits.load(std::memory_order_relaxed) << '\n';   // 8000
    return 0;
}
