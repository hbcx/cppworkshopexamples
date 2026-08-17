#include <barrier>
#include <iostream>
#include <thread>
#include <vector>

// A std::barrier is the reusable version of a latch. A fixed group of threads
// meets at it once per PHASE: each calls arrive_and_wait(), all block until the
// last arrives, then all are released -- and the barrier RESETS for the next
// phase. That is what a latch cannot do. Use it for iterative algorithms where
// every thread must finish step k before any starts step k+1 (simulations,
// image filters, anything with a data dependency between rounds).
int main() {
    const int N = 3;
    const int PHASES = 3;
    std::barrier sync(N);               // reusable; expects N arrivals each phase

    std::vector<long> work_done(N, 0);
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            for (int phase = 1; phase <= PHASES; ++phase) {
                work_done[i] += phase;          // this phase's contribution
                sync.arrive_and_wait();         // no one starts the next phase early
            }
        });
    }

    workers.clear();                    // join all

    long total = 0;
    for (long w : work_done) total += w;    // each did 1+2+3 = 6, times N = 3
    std::cout << N << " workers x " << PHASES << " phases, total = " << total
              << '\n';                            // 18
    return 0;
}
