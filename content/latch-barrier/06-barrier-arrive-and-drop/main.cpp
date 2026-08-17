#include <atomic>
#include <barrier>
#include <iostream>
#include <thread>
#include <vector>

// arrive_and_drop() lets a thread leave the barrier's group for good. It arrives
// at the CURRENT phase (so the others are released) and then permanently lowers
// the expected count for every FUTURE phase by one. This is how you handle work
// that shrinks over rounds: a thread whose share is finished bows out, and the
// remaining threads still synchronize correctly with the smaller count -- no need
// to rebuild the barrier.
int main() {
    const int N = 3;
    std::barrier sync(N);               // starts expecting 3 arrivals per phase
    std::atomic<int> phase2_participants{0};

    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            if (i == 2) {
                // Worker 2 does phase 1, then leaves the group. arrive_and_drop
                // counts for phase 1 AND drops the expected count to 2 afterwards.
                sync.arrive_and_drop();
                return;
            }
            // Workers 0 and 1 run two phases.
            sync.arrive_and_wait();             // phase 1: all three arrive
            phase2_participants.fetch_add(1);
            sync.arrive_and_wait();             // phase 2: barrier now expects 2
        });
    }

    workers.clear();                    // join all

    std::cout << "phase-2 participants after one dropped = "
              << phase2_participants.load() << '\n';    // 2
    return 0;
}
