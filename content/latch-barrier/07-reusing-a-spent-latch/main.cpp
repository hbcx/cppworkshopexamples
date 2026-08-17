#include <barrier>
#include <iostream>
#include <latch>
#include <thread>
#include <vector>

// Anti-pattern: reusing a latch for a second round.
//
// A std::latch is single-use. Once it reaches zero it stays there: wait() then
// returns immediately (no new synchronization), and calling count_down() again --
// pushing the counter below zero -- is undefined behaviour. So you cannot run a
// second "round" on the same latch:
//
//   std::latch l(N);
//   // round 1: workers count_down(), main wait()  -> counter now 0
//   // round 2: workers count_down() again  -> UB (counter goes below zero)
//   //          main wait()  -> returns at once, synchronizing nothing
//   // We do not run that version.
//
// For repeated rounds you need the reusable primitive: std::barrier. Below runs
// two rounds correctly with one barrier.
int main() {
    const int N = 3;
    const int ROUNDS = 2;
    std::barrier sync(N);               // reusable across rounds -- unlike a latch

    std::vector<long> work(N, 0);
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            for (int r = 0; r < ROUNDS; ++r) {
                work[i] += (i + 1);         // this round's work
                sync.arrive_and_wait();     // re-arms for the next round
            }
        });
    }
    workers.clear();

    long total = 0;
    for (long w : work) total += w;         // (1+2+3) * 2 rounds = 12
    std::cout << "barrier ran " << ROUNDS << " rounds, total = " << total
              << '\n';                          // 12
    return 0;
}
