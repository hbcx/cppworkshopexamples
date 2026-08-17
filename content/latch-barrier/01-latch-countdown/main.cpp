#include <iostream>
#include <latch>
#include <thread>
#include <vector>

// A std::latch is a one-shot countdown. You build it with a count, threads call
// count_down() as they finish, and a waiter calls wait() to block until the count
// reaches zero. It is the clean way to say "wait for these N tasks to all finish"
// -- no mutex, no condition variable, no shared counter of your own. Once it hits
// zero it stays there: a latch is single-use.
int main() {
    const int N = 4;
    std::latch done(N);                 // counts down from N to 0, exactly once

    std::vector<long> slots(N, 0);      // each worker writes its own slot
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&done, &slots, i] {
            slots[i] = (i + 1) * 10;    // ... do this task's work ...
            done.count_down();          // signal: this one is finished
        });
    }

    done.wait();                        // block until all N have counted down

    // Past wait() every worker's write is visible (count_down synchronizes with
    // wait), so we can read the results without any extra locking.
    long total = 0;
    for (long s : slots) total += s;
    std::cout << "all " << N << " workers done, total = " << total << '\n';  // 100
    return 0;
}
