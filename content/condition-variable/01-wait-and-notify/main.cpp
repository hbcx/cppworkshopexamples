#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // A std::condition_variable lets a thread SLEEP until some shared state becomes
    // true, instead of busy-checking it in a loop. It always works together with a
    // mutex (guarding the state) and a predicate (the condition being waited for).
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;          // the shared state the condition is about
    int payload = 0;

    std::thread consumer([&] {
        std::unique_lock<std::mutex> lock(m);
        // wait(lock, predicate) atomically releases the mutex and sleeps until
        // notified, then re-locks and checks the predicate -- looping until it
        // holds. Passing the predicate is what makes this correct against spurious
        // wakeups (waking for no reason) and lost wakeups (a notify that arrives
        // before the wait): if 'ready' is already true, wait returns at once.
        cv.wait(lock, [&] { return ready; });
        std::cout << "consumer got payload = " << payload << '\n';   // 42
    });

    // Producer: change the state UNDER THE LOCK, then notify. Setting it under the
    // same mutex the waiter uses is what prevents a lost wakeup.
    {
        std::lock_guard<std::mutex> lock(m);
        payload = 42;
        ready = true;
    }
    cv.notify_one();             // wake one waiter (the consumer)

    consumer.join();
    return 0;
}
