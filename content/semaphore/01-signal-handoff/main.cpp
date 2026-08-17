#include <iostream>
#include <semaphore>
#include <thread>

// A std::binary_semaphore is a std::counting_semaphore<1>: a permit counter that
// holds 0 or 1. acquire() takes the permit (blocking while the count is 0),
// release() gives it back (waking a waiter). Unlike a mutex it has NO owner -- the
// thread that releases need not be the one that acquired -- so it is a clean
// one-way signal from one thread to another: "you may proceed now".
int main() {
    std::binary_semaphore ready{0};     // no permit yet: the worker will block
    int payload = 0;

    std::jthread worker([&] {
        ready.acquire();                // wait here until main signals
        // Past acquire() main's write to payload is visible (release synchronizes
        // with acquire), so we read it with no extra lock.
        std::cout << "worker got payload = " << payload << '\n';   // 42
    });

    payload = 42;                       // ... prepare the data ...
    ready.release();                    // signal: the worker may proceed
    return 0;
}
