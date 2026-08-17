#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// A lock shows acquire and release doing their jobs together. Taking a lock must
// ACQUIRE -- so the new holder sees every write the previous holder made inside the
// critical section. Releasing must RELEASE -- so the next holder sees the writes this
// one just made. Here a tiny spinlock built from an atomic<bool> guards a plain
// counter: the exchange that takes the lock uses acquire, the store that frees it uses
// release, and the counter comes out exact.
struct SpinLock {
    std::atomic<bool> held{false};
    void lock() {
        // exchange returns the OLD value; keep trying until we flip it from false.
        while (held.exchange(true, std::memory_order_acquire)) {
        }
    }
    void unlock() {
        held.store(false, std::memory_order_release);
    }
};

int main() {
    SpinLock lock;
    long counter = 0;                   // plain data, protected by the lock

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k) {
                lock.lock();
                ++counter;              // safe: acquire/release bracket the section
                lock.unlock();
            }
        });
    }
    workers.clear();                    // join all

    std::cout << "spinlock-guarded counter = " << counter << '\n';   // 8000
    return 0;
}
