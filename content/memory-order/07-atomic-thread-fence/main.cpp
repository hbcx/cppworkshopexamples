#include <atomic>
#include <iostream>
#include <thread>

// std::atomic_thread_fence separates the ORDERING from the atomic operation. Instead
// of tagging a store with release, you do a plain relaxed store and place a standalone
// release fence before it; the consumer places an acquire fence after its relaxed load.
// A release fence paired with an acquire fence gives the same happens-before as a
// release store paired with an acquire load -- handy when one fence must order SEVERAL
// relaxed operations at once, rather than tagging each. (Here the published value is a
// relaxed atomic so the demo is self-contained; the same fence pattern also publishes
// ordinary non-atomic data.)
int main() {
    std::atomic<int> data{0};
    std::atomic<bool> ready{false};

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_relaxed)) {
        }
        std::atomic_thread_fence(std::memory_order_acquire);   // acquire fence
        // Past the acquire fence, the producer's writes before its release fence are
        // visible: the relaxed load reliably sees 99.
        std::cout << "consumer read data = "
                  << data.load(std::memory_order_relaxed) << '\n';   // 99
    });

    data.store(99, std::memory_order_relaxed);              // relaxed write...
    std::atomic_thread_fence(std::memory_order_release);    // ...published by a release fence
    ready.store(true, std::memory_order_relaxed);           // then the flag, relaxed
    return 0;
}
