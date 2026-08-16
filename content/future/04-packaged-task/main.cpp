#include <future>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

// std::packaged_task wraps a callable together with a future for its result.
// Calling the task runs the callable and stores what it returns into the future.
// This is exactly what a task queue needs: you can build a job, take its future
// to hand back to the caller, then move the job somewhere else to be run later.
// A std::thread cannot return a value and std::async runs the work right away;
// a packaged_task lets you SEPARATE "make the job" from "run the job".
long square(long n) { return n * n; }

int main() {
    std::queue<std::packaged_task<long()>> jobs;
    std::vector<std::future<long>> results;

    // Build three jobs. For each we take its future now, then queue the job to
    // run later. bind the argument in so every job is a callable of () -> long.
    for (long n = 1; n <= 3; ++n) {
        std::packaged_task<long()> task([n] { return square(n); });
        results.push_back(task.get_future());   // the caller's handle to result
        jobs.push(std::move(task));              // packaged_task is move-only
    }

    // A worker drains the queue and runs each job. Invoking the task fulfils its
    // future; the worker never needs to know who is waiting on the result.
    std::thread worker([&jobs] {
        while (!jobs.empty()) {
            std::packaged_task<long()> job = std::move(jobs.front());
            jobs.pop();
            job();                               // run it -> future becomes ready
        }
    });

    long total = 0;
    for (auto& f : results) total += f.get();    // collect 1 + 4 + 9
    worker.join();

    std::cout << "sum of squares = " << total << '\n';   // 14
    return 0;
}
