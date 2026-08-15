#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // A std::mutex is, at bottom, just two operations: lock() and unlock(). Between
    // them at most one thread can be running -- that region is the critical section.
    // This example calls them DIRECTLY to show the raw mechanism.
    //
    // In real code you do NOT lock by hand like this: a return or an exception
    // between lock() and unlock() would skip the unlock and leave the mutex held
    // forever (a deadlock -- see the anti-patterns). The very next example wraps the
    // pair in std::lock_guard, which unlocks automatically; that is what you
    // actually use. This one exists only so you can see what lock_guard does for you.
    std::mutex m;
    long counter = 0;

    auto work = [&] {
        for (int k = 0; k < 100000; ++k) {
            m.lock();          // acquire: blocks until no other thread holds it
            ++counter;         // critical section: exactly one thread at a time
            m.unlock();        // release: let another waiting thread in
        }
    };

    std::thread a(work), b(work);
    a.join();
    b.join();

    std::cout << "counter = " << counter << '\n';   // 200000, no lost updates
    return 0;
}
