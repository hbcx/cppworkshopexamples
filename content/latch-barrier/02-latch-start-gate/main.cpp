#include <iostream>
#include <latch>
#include <thread>
#include <vector>

// The other way to use a latch: as a START gate. Build a latch of 1, have every
// worker wait() on it, do your setup, then count_down() once to release them all
// at the same moment. This is handy for benchmarks and staged startup, where you
// want the workers created and parked, then all let go together rather than
// trickling in as each thread happens to start.
int main() {
    const int N = 4;
    std::latch go(1);                   // opens when counted down once

    std::vector<long> slots(N, 0);
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&go, &slots, i] {
            go.wait();                  // park here until the gate opens
            slots[i] = (i + 1);         // the real work, started in sync
        });
    }

    // ... all workers are now waiting; finish setup here ...
    go.count_down();                    // open the gate: everyone proceeds at once

    workers.clear();                    // join every jthread (destructor)

    long total = 0;
    for (long s : slots) total += s;
    std::cout << "workers released together, total = " << total << '\n';  // 10
    return 0;
}
