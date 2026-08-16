#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>

// Anti-pattern: expecting a stop to wake a plain std::condition_variable.
//
// std::condition_variable has NO stop_token wait overload. A worker blocked in
// cv.wait(lock, pred) will not wake just because request_stop() was called, so on
// shutdown it hangs and the jthread's join blocks forever:
//
//   std::condition_variable cv;                 // NOT condition_variable_any
//   worker: cv.wait(lock, [&]{ return has_work; });   // no token involved
//   worker.request_stop();   // cv never notified -> stays asleep -> join HANGS
//
// Two fixes: use std::condition_variable_any with the wait(lock, token, pred)
// overload (see the interruptible-wait example), OR keep the plain cv and bridge
// the stop with a std::stop_callback that notifies it. This shows the bridge.

int main() {
    std::mutex m;
    std::condition_variable cv;         // plain cv, on purpose
    bool has_work = false;              // guarded by m; stays false
    std::atomic<bool> stopping{false};
    bool woke_on_stop = false;

    std::jthread worker([&](std::stop_token st) {
        std::unique_lock<std::mutex> lock(m);
        // Bridge: when a stop is requested, flip an atomic and notify the plain
        // cv. The callback does NOT lock m, so it is safe even if it fires
        // synchronously here (stop already requested) while we hold the lock.
        std::stop_callback cb(st, [&] {
            stopping.store(true);
            cv.notify_one();
        });
        cv.wait(lock, [&] { return has_work || stopping.load(); });
        if (stopping.load()) woke_on_stop = true;
    });

    worker.request_stop();   // fires the callback, which notifies the plain cv
    worker.join();

    std::cout << "plain cv woken via stop_callback bridge = " << std::boolalpha
              << woke_on_stop << '\n';          // true
    return 0;
}
