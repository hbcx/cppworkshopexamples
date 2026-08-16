#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // ANTI-PATTERN: calling cv.wait with a lock that does NOT currently hold the
    // mutex. wait() requires that the unique_lock owns the mutex on entry -- it
    // releases the mutex while sleeping and re-acquires it on wake. Passing an
    // unlocked lock (one built with std::defer_lock, or one you already unlock()ed)
    // is undefined behaviour.
    //
    //   std::unique_lock<std::mutex> lock(m, std::defer_lock);  // NOT locked
    //   cv.wait(lock, pred);      // UB: wait requires an owned (locked) mutex
    //
    // We do NOT run that: it is undefined behaviour. The waiter must hold the lock.

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;

    std::thread consumer([&] {
        std::unique_lock<std::mutex> lock(m);   // LOCKED on construction -- correct
        cv.wait(lock, [&] { return ready; });   // wait owns the mutex, as required
        std::cout << "consumer proceeded\n";
    });

    {
        std::lock_guard<std::mutex> lock(m);
        ready = true;
    }
    cv.notify_one();

    consumer.join();

    // The mental model: you hand wait() a LOCKED mutex; it releases it for you while
    // it sleeps and returns it locked when it wakes. You never call wait on a mutex
    // you do not currently hold, and you keep the lock for the predicate re-checks.
    return 0;
}
