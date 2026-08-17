#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // notify_one wakes ONE waiting thread; notify_all wakes EVERY thread waiting on
    // this atomic. Use notify_all when one change should release many waiters at
    // once -- a start gate that holds all workers until the signal, a shutdown flag
    // every thread is parked on.

    std::atomic<int> gate{0};        // 0 = closed, workers wait here
    std::atomic<int> passed{0};      // how many got through, counted race-free

    std::vector<std::thread> workers;
    for (int i = 0; i < 8; ++i)
        workers.emplace_back([&] {
            gate.wait(0);                 // block while the gate is still closed
            passed.fetch_add(1);          // released -- record it atomically
        });

    // All eight threads are now parked on gate.wait(0). Open the gate once and wake
    // every one of them together. A single notify_one here would release just one
    // worker and leave the other seven parked forever.
    gate.store(1);
    gate.notify_all();

    for (auto& t : workers) t.join();

    std::cout << "workers released = " << passed.load() << '\n';   // 8
    return 0;
}
