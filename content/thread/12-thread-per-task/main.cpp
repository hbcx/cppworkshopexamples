#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Pretend each task is one small unit of work.
static long task_work(int id) {
    long s = 0;
    for (int k = 0; k < 50; ++k) s += id + k;
    return s;
}

int main() {
    std::cout << std::boolalpha;
    const int TASKS = 64;

    // ANTI-PATTERN: one std::thread per task. Creating and destroying a thread costs
    // far more than a small task itself, and running many more threads than the
    // machine has cores just piles on context-switch overhead (oversubscription).
    // With 64 tasks it merely wastes work; with thousands or millions it collapses,
    // and can hit the OS thread limit and throw. It still gives the right answer,
    // which is exactly why the cost is easy to miss.
    std::atomic<long> naive_total{0};
    std::vector<std::thread> per_task;
    per_task.reserve(TASKS);
    for (int i = 0; i < TASKS; ++i)
        per_task.emplace_back([i, &naive_total] { naive_total.fetch_add(task_work(i)); });
    for (auto& t : per_task) t.join();

    // FIX: a fixed pool sized to the hardware. Each worker pulls MANY tasks, so the
    // thread count stays near the core count no matter how many tasks there are --
    // the per-thread cost is paid a handful of times, not once per task.
    unsigned workers = std::thread::hardware_concurrency();
    if (workers == 0) workers = 4;
    std::atomic<long> pooled_total{0};
    std::vector<std::thread> pool;
    for (unsigned w = 0; w < workers; ++w)
        pool.emplace_back([w, workers, &pooled_total] {
            long local = 0;
            for (int i = static_cast<int>(w); i < TASKS; i += static_cast<int>(workers))
                local += task_work(i);            // this worker's strided slice
            pooled_total.fetch_add(local);
        });
    for (auto& t : pool) t.join();

    std::cout << "per-thread-per-task total = " << naive_total.load() << '\n';
    std::cout << "pooled total              = " << pooled_total.load() << '\n';
    std::cout << "same result? " << (naive_total.load() == pooled_total.load()) << '\n';

    // The pool does the same work with a bounded number of threads. For real
    // workloads, prefer a ready-made thread pool or a parallel algorithm (see the
    // execution-policies section) over hand-spawning a thread per task.
    return 0;
}
