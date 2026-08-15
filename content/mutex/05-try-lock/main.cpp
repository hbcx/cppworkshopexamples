#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // try_lock() attempts to take the mutex WITHOUT blocking: it returns true and
    // locks if the mutex was free, or returns false immediately if another thread
    // holds it. So a thread can do other work, or skip, instead of waiting.
    std::mutex m;

    m.lock();                         // main deliberately holds the mutex

    std::thread worker([&] {
        if (m.try_lock()) {           // fails: main is holding it
            std::cout << "worker: got the lock\n";
            m.unlock();
        } else {
            std::cout << "worker: mutex busy, skipped the work\n";
        }
    });
    worker.join();                    // worker runs and finishes while main holds 'm'

    m.unlock();                       // now main releases it

    // With the mutex free, a try_lock now succeeds.
    if (m.try_lock()) {
        std::cout << "main: got the lock after releasing\n";
        m.unlock();
    }

    // try_lock is how you avoid blocking: attempt the lock, and if it is contended,
    // do something else rather than wait. The timed variants try_lock_for and
    // try_lock_until (on std::timed_mutex) wait up to a deadline before giving up.
    // Note: on success you own the lock, so pair it with an unlock -- or, better,
    // hand the mutex to std::unique_lock with std::try_to_lock so RAII unlocks it.
    return 0;
}
