#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Atomically raise `target` to candidate if candidate is larger -- an "atomic
// max", which the fetch_ family does not provide. Built from a CAS loop.
static void atomic_max(std::atomic<int>& target, int candidate) {
    int current = target.load();
    // compare_exchange_weak: if target still equals `current`, set it to candidate
    // and return true; otherwise load the fresh value into `current` and return
    // false, so we retry with the up-to-date value.
    while (candidate > current &&
           !target.compare_exchange_weak(current, candidate)) {
        // current was refreshed by the failed exchange; loop condition re-checks.
    }
}

int main() {
    std::atomic<int> high{0};

    std::vector<std::thread> pool;
    for (int i = 0; i < 8; ++i)
        pool.emplace_back([&, i] { atomic_max(high, i * 10); });   // proposes 0..70
    for (auto& t : pool) t.join();

    // Deterministic: the largest proposal wins, no matter the interleaving.
    std::cout << "high = " << high.load() << '\n';   // 70

    // compare_exchange is the general tool: any read-modify-write you can express
    // as "compute a new value from the old, install it if the old has not changed"
    // becomes lock-free with a CAS loop -- atomic max/min, saturating add, lock-free
    // stacks. Use the _weak form in loops (it may fail spuriously but is cheaper);
    // use _strong for a single, non-looping attempt.
    return 0;
}
