#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: reaching for a latch or barrier when a plain join is the answer.
//
// A latch shines when you must wait for signals from threads you do NOT own and
// cannot join -- a thread pool, detached workers, callbacks. But if you own the
// threads and are going to join them anyway, a latch on top is pure ceremony:
// join already waits for completion AND gives you the happens-before to read the
// results. Adding a latch(N) counted down at the end of each worker duplicates
// exactly what the join does.
//
//   std::latch done(N);
//   workers: ... work ...; done.count_down();     // redundant
//   done.wait();                                    // join will do this anyway
//   for (auto& t : threads) t.join();
//
// The fix: just join (or let std::jthread auto-join) and then read the results.
int main() {
    const int N = 4;
    std::vector<long> slots(N, 0);

    {
        std::vector<std::jthread> workers;
        for (int i = 0; i < N; ++i) {
            workers.emplace_back([&slots, i] {
                slots[i] = (i + 1) * 10;    // ... the work ...
            });
        }
    }   // <-- all jthreads auto-join here; no latch needed

    // After the join, every write is visible -- the same guarantee a latch gives.
    long total = 0;
    for (long s : slots) total += s;
    std::cout << "joined workers, total = " << total << '\n';   // 100

    // Rule of thumb: join to wait for threads you own; a latch to wait for signals
    // from threads you do NOT own; a barrier for a group that meets REPEATEDLY.
    return 0;
}
