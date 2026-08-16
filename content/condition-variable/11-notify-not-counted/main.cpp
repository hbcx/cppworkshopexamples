#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // ANTI-PATTERN: treating notify as if it were remembered or counted. A notify is
    // an EDGE: it wakes threads already waiting and is otherwise lost. It is not a
    // token that piles up, so you cannot notify N times up front and expect N future
    // waits to each return. The truth must live in shared STATE, not in the count of
    // notifications.
    //
    //   // BROKEN: 3 notifies sent before the consumer waits -> all lost; the
    //   // consumer then blocks on its first wait forever.
    //   for (int i = 0; i < 3; ++i) cv.notify_one();
    //
    // FIX: keep a COUNT of pending items in shared state and let the predicate read
    // it. Then it does not matter whether a notify arrived before or after a wait --
    // the waiter checks the count, which is level-triggered and never lost.

    std::mutex m;
    std::condition_variable cv;
    int pending = 0;               // the state that actually records the work
    long consumed = 0;

    std::thread consumer([&] {
        for (int done = 0; done < 3; ++done) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&] { return pending > 0; });
            --pending;
            ++consumed;
        }
    });

    // Produce three items -- possibly all before the consumer ever waits. Because
    // each increments 'pending' under the lock, none is lost even if its notify was.
    for (int i = 0; i < 3; ++i) {
        {
            std::lock_guard<std::mutex> lock(m);
            ++pending;
        }
        cv.notify_one();
    }

    consumer.join();
    std::cout << "consumed = " << consumed << '\n';   // 3

    // notify is edge-triggered (wakes current waiters); shared state is level-
    // triggered (still there whenever a thread looks). Correct code decides on the
    // STATE and uses notify only as a nudge to go re-check it.
    return 0;
}
