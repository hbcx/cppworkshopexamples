#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // ANTI-PATTERN: waiting on a condition variable WITHOUT a predicate -- a bare
    // cv.wait(lock), or an `if` around it instead of a loop. Two things break it:
    //   * LOST WAKEUP: if the notify happens BEFORE the thread reaches wait(), the
    //     bare wait sleeps forever -- a notify only wakes threads already waiting, it
    //     is not remembered.
    //   * SPURIOUS WAKEUP: wait() may return with no notify at all, so code after a
    //     predicate-less wait runs when the condition is NOT actually true.
    //
    //   // BROKEN:
    //   std::unique_lock<std::mutex> lock(m);
    //   cv.wait(lock);            // no predicate: misses an early notify -> hangs,
    //   use(payload);             // and may run on a spurious wakeup
    //
    // We do NOT run that -- a lost wakeup would hang the build forever.

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    int payload = 0;

    // Producer runs FIRST and notifies before the consumer waits -- the exact race
    // that makes a bare wait() lose the wakeup.
    {
        std::lock_guard<std::mutex> lock(m);
        payload = 42;
        ready = true;
    }
    cv.notify_one();              // sent before anyone is waiting

    // FIX: the predicate form. wait re-checks 'ready' immediately; since it is
    // already true it returns at once instead of sleeping -- there is no wakeup to
    // lose. It also loops on spurious wakeups, so it never falls through when the
    // condition is false.
    std::thread consumer([&] {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return ready; });   // safe against lost AND spurious wakeups
        std::cout << "consumer got payload = " << payload << '\n';   // 42
    });

    consumer.join();
    return 0;
}
