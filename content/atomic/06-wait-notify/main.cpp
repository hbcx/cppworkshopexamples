#include <atomic>
#include <iostream>
#include <thread>

int main() {
    // Before C++20, waiting for an atomic to change meant a BUSY-SPIN loop:
    //   while (flag.load() == old) { }   // burns a whole CPU core doing nothing
    // C++20 adds wait/notify so a thread can BLOCK (the OS parks it, using no CPU)
    // until the value changes and it is woken -- like a condition variable, but
    // built into the atomic and with no separate mutex.

    std::atomic<int> state{0};   // 0 = not ready yet

    std::thread consumer([&] {
        // wait(old) blocks while state == old. It returns only once the value is
        // DIFFERENT from old AND a notify has been sent. It also re-checks the
        // value itself, so a spurious wake does not fall through -- unlike a raw
        // condition variable, you do not need your own while-loop predicate here.
        state.wait(0);
        std::cout << "consumer woke up, state = " << state.load() << '\n';   // 1
    });

    // Produce the data, publish it by changing the atomic, then wake the waiter.
    // The store must come BEFORE notify_one -- notify only wakes threads; it is the
    // changed value they observe. notify_one wakes a single waiting thread.
    state.store(1);
    state.notify_one();

    consumer.join();

    // Why wait/notify over a busy-spin: a spin loop keeps a core at 100% and starves
    // other work; wait parks the thread so the CPU is free until there is something
    // to do. Why over a condition_variable: no separate mutex and no lost-wakeup
    // window, because the wait is tied to the atomic's own value.
    return 0;
}
