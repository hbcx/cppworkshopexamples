#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>

// The clean way to shut down a thread that is BLOCKED on a condition variable.
// std::condition_variable_any has a wait overload that also takes a stop_token:
// it waits until the predicate holds OR a stop is requested, and wakes for the
// stop. That removes the hand-rolled "stopping" flag a plain condition_variable
// needs in its predicate -- the stop_token IS the shutdown signal.
int main() {
    std::mutex m;
    std::condition_variable_any cv;
    bool has_work = false;              // guarded by m; stays false here
    bool woke_on_stop = false;

    std::jthread worker([&](std::stop_token st) {
        std::unique_lock<std::mutex> lock(m);
        // Returns the predicate's value: true if work arrived, false if it woke
        // because a stop was requested. No "|| stopping" in the predicate.
        bool got_work = cv.wait(lock, st, [&has_work] { return has_work; });
        if (!got_work) woke_on_stop = true;   // woke due to cancellation
    });

    // We never post work. Without the stop_token overload this wait would block
    // forever; request_stop() wakes it. (This is race-free even if the stop lands
    // before the worker reaches wait: the overload checks the token on entry.)
    worker.request_stop();
    worker.join();

    std::cout << "worker woke on cancel = " << std::boolalpha << woke_on_stop
              << '\n';                        // true
    return 0;
}
