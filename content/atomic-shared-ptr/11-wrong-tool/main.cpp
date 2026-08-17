#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

// Anti-pattern: reaching for atomic<shared_ptr> when a mutex is the right tool.
//
// atomic<shared_ptr> shines for ONE read-mostly value swapped wholesale. It is the
// wrong tool when:
//   * you must update several independent pieces of shared state together and keep
//     them consistent -- a single atomic pointer cannot span two objects
//     transactionally;
//   * the workload is write-heavy -- the specialization is not lock-free, so every
//     store contends on an internal lock, and a std::mutex is just as fast and clearer;
//   * the pointer is never actually shared between threads -- then a plain shared_ptr
//     copy is enough and cheaper.
//
// When two related values must move as one, guard them with a std::mutex. Below a
// transfer keeps two accounts consistent under a single lock -- something no lone
// atomic<shared_ptr> can express.
int main() {
    std::mutex m;
    long account_a = 1000;
    long account_b = 0;

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < 100; ++k) {
                std::lock_guard<std::mutex> guard{m};   // both moves under one lock
                account_a -= 1;
                account_b += 1;
            }
        });
    }
    workers.clear();                    // join all

    std::cout << "a + b conserved = " << std::boolalpha
              << (account_a + account_b == 1000)
              << " (a=" << account_a << ", b=" << account_b << ")\n";   // true (a=200, b=800)
    return 0;
}
