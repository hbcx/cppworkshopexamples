#include "queue.hpp"

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

using namespace std::chrono_literals;

// Demo of the non-blocking and time-limited operations. Every line is printed
// from the main thread (worker results are read only after join), so the output
// is deterministic: it shows the SEMANTICS -- what each call returns -- not the
// timing, which is what varies between runs.
int main() {
    // --- non-blocking try_push / try_pop: semantics on a capacity-2 queue ----
    BoundedBlockingQueue<int> q(2);
    std::cout << "try_push 10:        " << q.try_push(10) << '\n'; // 1 (room)
    std::cout << "try_push 20:        " << q.try_push(20) << '\n'; // 1 (room)
    std::cout << "try_push 30 (full): " << q.try_push(30) << '\n'; // 0 (full)

    std::optional<int> a = q.try_pop();
    std::optional<int> b = q.try_pop();
    std::optional<int> c = q.try_pop(); // now empty
    std::cout << "try_pop:            " << (a ? *a : -1) << '\n'; // 10
    std::cout << "try_pop:            " << (b ? *b : -1) << '\n'; // 20
    std::cout << "try_pop (empty):    " << (c ? "value" : "empty") << '\n';

    // --- pop_for that times out because nothing arrives ---------------------
    std::optional<int> timedOut = q.pop_for(20ms);
    std::cout << "pop_for 20ms empty: " << (timedOut ? "value" : "timed out") << '\n';

    // --- pop_for that succeeds because an item arrives well within the wait --
    BoundedBlockingQueue<int> q2(4);
    std::thread producer([&] { q2.push(99); });
    std::optional<int> arrived = q2.pop_for(5s); // 5s >> the push, so this wins
    producer.join();
    std::cout << "pop_for 5s, got:    " << (arrived ? *arrived : -1) << '\n'; // 99

    // --- a waiter blocked in pop_for is released promptly by close() --------
    BoundedBlockingQueue<int> q3(4);
    std::optional<int> afterClose;
    std::thread waiter([&] { afterClose = q3.pop_for(5s); });
    q3.close();       // wakes the waiter; predicate sees closed -> returns
    waiter.join();    // afterClose is safely visible after the join
    std::cout << "pop_for after close: " << (afterClose ? "value" : "empty (closed)") << '\n';
    return 0;
}
