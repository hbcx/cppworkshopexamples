#include <iostream>
#include <latch>
#include <semaphore>
#include <thread>
#include <vector>

// Anti-pattern: reaching for a semaphore when a purpose-built primitive says the
// intent directly.
//
// A counting_semaphore is a general permit counter, so you CAN bend it into other
// coordination jobs -- but doing so hides the intent and is easy to get wrong:
//   * "Wait for N tasks to finish" by starting a semaphore at 0, releasing once per
//     finished task, then acquiring N times on the waiter -- that is a std::latch
//     done by hand, and you must get the N exactly right or acquire() hangs.
//   * "Signal a result is ready once" -- that is a std::future/std::promise, which
//     also carries the value and any exception across the thread boundary.
//   * "Rendezvous a fixed group every round" -- that is a std::barrier.
//
// Use a semaphore when the thing you count really is interchangeable PERMITS: pool
// slots, rate limits, bounded-buffer slots. For "wait for N to finish", say so with
// a latch. Below is the latch version -- shorter and self-explanatory.
int main() {
    const int N = 5;
    std::latch done(N);

    std::vector<long> slots(N, 0);
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            slots[i] = (i + 1) * 4;
            done.count_down();               // says "one task finished" directly
        });
    }
    done.wait();                             // says "wait for all N" directly

    long total = 0;
    for (long s : slots) total += s;
    std::cout << "latch expresses 'wait for " << N << "' clearly, total = " << total << '\n';  // 60
    return 0;
}
