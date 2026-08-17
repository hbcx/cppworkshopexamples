#include <atomic>
#include <iostream>
#include <thread>

// atomic_ref carries the full memory-order model, so you can publish data through a
// plain flag you do not own. The producer fills a payload, then stores a ready flag
// with release; the consumer waits for the flag with acquire. The release/acquire
// pair guarantees that once the consumer sees the flag set, every write the producer
// made before the store is visible -- all on ordinary variables, reached via
// atomic_ref.
int main() {
    int payload = 0;                    // plain data
    bool ready = false;                 // plain flag

    std::jthread consumer([&] {
        std::atomic_ref<bool> flag{ready};
        while (!flag.load(std::memory_order_acquire)) {
            // spin until published (a real program would back off, or use wait())
        }
        // The acquire load saw the producer's release store, so payload's write
        // happens-before this read: it is safe to read payload plainly here.
        std::cout << "consumer read payload = " << payload << '\n';   // 99
    });

    payload = 99;                       // ... produce the data ...
    std::atomic_ref<bool>{ready}.store(true, std::memory_order_release);   // publish
    return 0;
}
