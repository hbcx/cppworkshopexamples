#include <atomic>
#include <barrier>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: a barrier whose expected count does not match the arrivals.
//
// A std::barrier(K) completes a phase only when exactly K threads arrive. Get K
// wrong and it breaks:
//
//   * TOO HIGH -- barrier(N+1) but only N threads arrive (you sized it for a
//     coordinator that never calls arrive_and_wait). The phase never completes;
//     every worker deadlocks waiting for the arrival that will not come.
//   * TOO LOW -- barrier(N-1) with N threads. The phase completes and releases
//     the group before the last thread arrives, so its late arrival lands in the
//     NEXT phase, corrupting the synchronization.
//
// We do not run the deadlocking version. The fix is to derive the count from the
// actual number of participating threads -- one source of truth, not a literal.
int main() {
    const int N = 3;
    std::barrier sync(N);               // count == number of threads that arrive

    std::atomic<int> completed_phase{0};
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&] {
            sync.arrive_and_wait();     // all N arrive -> phase completes cleanly
            completed_phase.fetch_add(1);
        });
    }
    workers.clear();

    std::cout << "threads that passed the barrier = " << completed_phase.load()
              << " of " << N << '\n';           // 3 of 3
    return 0;
}
