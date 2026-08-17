#include <atomic>
#include <iostream>
#include <thread>

// release/acquire is the workhorse of lock-free code: it publishes data from one
// thread to another at lower cost than seq_cst. The producer writes its data, then
// does a store with memory_order_release on a flag. The consumer loads the flag with
// memory_order_acquire; once it sees the released value, the release STORE
// synchronizes-with the acquire LOAD, and everything the producer wrote BEFORE the
// release is guaranteed visible to the consumer AFTER the acquire. That edge is called
// happens-before.
int main() {
    int payload = 0;                    // plain data, published through the flag
    std::atomic<bool> ready{false};

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {
            // spin until the producer publishes (real code would back off or wait)
        }
        // The acquire saw the release, so payload's write happens-before this read.
        std::cout << "consumer read payload = " << payload << '\n';   // 99
    });

    payload = 99;                                       // ... produce the data ...
    ready.store(true, std::memory_order_release);       // publish it
    return 0;
}
