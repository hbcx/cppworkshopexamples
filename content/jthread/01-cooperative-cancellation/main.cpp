#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// std::jthread is C++20's cooperative-cancellation thread. It does two things a
// plain std::thread does not: it JOINS itself in its destructor (so you can never
// forget), and it hands its callable a std::stop_token. When the jthread is
// destroyed it first calls request_stop() and THEN joins -- so a long-running
// worker that checks its stop_token will be asked to stop and will bow out on its
// own, all automatically. There is no forced kill in C++: the worker must
// cooperate by checking the token at a safe point.
int main() {
    std::atomic<bool> saw_stop{false};

    {
        // If the callable's first parameter is std::stop_token, jthread passes in
        // its own token. The worker loops until a stop is requested.
        std::jthread worker([&saw_stop](std::stop_token st) {
            while (!st.stop_requested()) {
                // ... do a chunk of work, then loop back and re-check the token ...
            }
            saw_stop.store(true);   // left the loop because stop was requested
        });
        // We do our own thing here; the worker runs in the background.
    }   // <-- jthread destructor: request_stop() first, then join(). The worker
        //     sees the flag, leaves its loop, and we wait for it -- no code needed.

    std::cout << "worker cancelled cooperatively = " << std::boolalpha
              << saw_stop.load() << '\n';                 // true
    return 0;
}
