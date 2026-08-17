#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Every atomic operation takes a std::memory_order argument; when you leave it out,
// the default is memory_order_seq_cst (sequentially consistent) -- the strongest and
// easiest order to reason about. All seq_cst operations across all threads behave as
// if they happened in one single global sequence that every thread agrees on. It is
// the safe default: reach for a weaker order only when you have a concrete reason and
// have checked the reasoning. Here the increments simply use the default.
int main() {
    std::atomic<long> counter{0};       // operations below default to seq_cst

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k) {
                ++counter;              // seq_cst fetch_add -- the default
            }
        });
    }
    workers.clear();                    // join all

    std::cout << "seq_cst counter = " << counter.load() << '\n';   // 8000
    return 0;
}
