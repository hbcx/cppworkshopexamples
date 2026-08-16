#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    std::cout << std::boolalpha;
    // ANTI-PATTERN: changing the shared state WITHOUT holding the mutex, then
    // notifying. There is a race between the waiter checking its predicate and going
    // to sleep: if the producer flips the flag (unlocked) and notifies inside that
    // window, the waiter has already decided to sleep and the notify is lost -- so it
    // blocks forever even though the condition is now true.
    //
    //   // BROKEN producer:
    //   ready = true;             // set WITHOUT the mutex
    //   cv.notify_one();          // may land in the gap between the waiter's check
    //                             // and its sleep -> lost wakeup
    //
    // Setting 'ready' under the SAME mutex the waiter holds while checking closes the
    // gap: the flip cannot happen between the waiter's check and its sleep, because
    // the waiter holds the lock across both. We run only the correct version.

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;

    std::thread consumer([&] {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return ready; });
        std::cout << "consumer proceeded, ready = " << ready << '\n';   // true
    });

    // FIX: change the state under the lock, THEN notify. (The notify call itself may
    // be made with the lock held or just after releasing it -- but the STATE change
    // must be inside the lock.)
    {
        std::lock_guard<std::mutex> lock(m);
        ready = true;
    }
    cv.notify_one();

    consumer.join();
    return 0;
}
