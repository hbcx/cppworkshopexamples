#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    std::cout << std::boolalpha;
    // ANTI-PATTERN: reading the shared state the condition is about WITHOUT holding
    // the mutex -- an unlocked peek like `if (ready) use(payload);`, or using the
    // guarded payload after the lock has been released. The writer sets that state
    // under the lock, so an unlocked read races with it: undefined behaviour, and it
    // may see a stale or half-written value. ThreadSanitizer flags exactly this.
    //
    //   // BROKEN: unlocked peek at guarded state
    //   if (ready) use(payload);          // data race with the producer's write
    //
    // The predicate inside cv.wait is evaluated WHILE the lock is held, which is why
    // it is safe; every OTHER access to the same state must also take the lock.

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;
    int payload = 0;

    std::thread consumer([&] {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [&] { return ready; });         // predicate read UNDER the lock
        std::cout << "payload under lock = " << payload << '\n';   // 42, still locked
    });

    {
        std::lock_guard<std::mutex> lock(m);
        payload = 42;
        ready = true;
    }
    cv.notify_one();

    consumer.join();

    // FIX for any reader outside the wait: take the mutex before touching the state.
    {
        std::lock_guard<std::mutex> lock(m);
        std::cout << "final ready = " << ready << '\n';   // true, read under the lock
    }
    return 0;
}
