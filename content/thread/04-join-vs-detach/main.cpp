#include <atomic>
#include <iostream>
#include <thread>

int main() {
    // JOIN: you wait for the thread and then rely on its results. The worker's
    // lifetime is tied to this scope -- the join point.
    int joined_result = 0;
    std::thread a([&] { joined_result = 111; });
    a.join();
    std::cout << "join:   result = " << joined_result << '\n';   // 111

    // DETACH: you cut the thread loose to run on its own -- "fire and forget". The
    // std::thread object stops owning it, and you can never join it again. The
    // trade-off: you gave up the one built-in way to know when it finished, so if
    // you still need its result you must synchronize YOURSELF. Here a small atomic
    // handshake does that: the worker publishes its result and sets a done flag.
    std::atomic<bool> done{false};
    std::atomic<int> detached_result{0};
    std::thread b([&] {
        detached_result.store(42);
        done.store(true);                 // publish "I have finished"
    });
    b.detach();                           // b no longer owns the thread

    while (!done.load())                   // WE wait, since we cannot join it
        std::this_thread::yield();         // yield instead of spinning hot
    std::cout << "detach: result = " << detached_result.load() << '\n';   // 42

    // Rule of thumb: prefer join. Reach for detach only for genuine fire-and-forget
    // work whose result and finish time you do not need to track -- and even then,
    // a detached thread must not outlive the data it refers to (see the anti-
    // patterns). In modern code, prefer std::jthread, which joins itself.
    return 0;
}
