#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// memory_order_acq_rel is for a read-modify-write that must synchronize in BOTH
// directions at once: the ACQUIRE half sees the writes of whoever acted before it, and
// the RELEASE half publishes its own writes to whoever acts next -- in one atomic step.
// A baton relay makes it concrete: workers take strict turns; each waits for its turn,
// extends a plain baton the previous worker left, and advances the turn with an
// acq_rel fetch_add. The acquire imports the predecessor's write; the release publishes
// this one's write for the successor.
int main() {
    const int N = 4;
    std::atomic<int> turn{0};
    long baton = 0;                     // plain data, passed hand to hand

    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            while (turn.load(std::memory_order_acquire) != i) {
                // spin until it is my turn
            }
            baton += (i + 1) * 10;      // read what the last worker left, add my part
            turn.fetch_add(1, std::memory_order_acq_rel);   // publish my write + advance the turn
        });
    }
    workers.clear();                    // join all

    std::cout << "baton relayed through " << N << " workers, total = " << baton << '\n';  // 100
    return 0;
}
