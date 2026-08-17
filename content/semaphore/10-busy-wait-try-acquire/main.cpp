#include <atomic>
#include <iostream>
#include <semaphore>
#include <thread>

// Anti-pattern: spinning on try_acquire() instead of blocking on acquire().
//
// try_acquire() returns immediately -- true if it got a permit, false if none was
// free. Looping on it to "wait" turns the wait into a busy-spin that pins a CPU
// core doing nothing until a permit appears:
//
//   while (!sem.try_acquire()) { }        // burns 100% of a core while it waits
//
// acquire() instead puts the thread to sleep and lets the OS wake it when a permit
// is released -- no wasted cycles, and usually lower latency too. Use try_acquire()
// only for a real "check and move on if busy", try_acquire_for()/try_acquire_until()
// for a bounded wait, and plain acquire() when you simply need to wait.
//
// Below the worker blocks in acquire(); nothing spins.
int main() {
    std::binary_semaphore go{0};
    std::atomic<int> order{0};
    int worker_step = 0, main_step = 0;

    std::jthread worker([&] {
        go.acquire();                       // sleeps here -- no spin -- until released
        worker_step = ++order;              // records when it actually woke
    });

    // ... main does its setup ...
    main_step = ++order;                    // main reaches its point first
    go.release();                           // now wake the worker

    worker.join();
    std::cout << "main step " << main_step << ", worker step " << worker_step
              << " (worker waited without spinning)\n";   // main step 1, worker step 2
    return 0;
}
