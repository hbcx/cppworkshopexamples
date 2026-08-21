#include "thread_pool.hpp"

#include <atomic>
#include <iostream>

// Demo: submit many small tasks to the pool and let it run them on a few worker
// threads. Each task adds its number to a shared atomic total; because addition
// does not care about order, the final total is deterministic and lets us check
// that every submitted task actually ran.
int main() {
    const int workers = 4;
    const int taskCount = 2000;

    std::atomic<long long> total{0};

    {
        ThreadPool pool(workers, /*queueCapacity=*/128);
        for (int i = 1; i <= taskCount; ++i)
            pool.submit([&total, i] { total.fetch_add(i, std::memory_order_relaxed); });
        pool.shutdown(); // drains every queued task, then joins the workers
    }

    const long long expected = 1LL * taskCount * (taskCount + 1) / 2;
    std::cout << "tasks submitted: " << taskCount << '\n';
    std::cout << "sum of task ids: " << total.load() << '\n';
    std::cout << "expected sum:    " << expected << '\n';
    std::cout << "result: " << (total.load() == expected ? "OK" : "MISMATCH") << '\n';
    return 0;
}
