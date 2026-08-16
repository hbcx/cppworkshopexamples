#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// You do not have to wait for the destructor: a jthread owns a std::stop_source,
// and you can inspect it and trigger a stop early through the jthread itself.
// request_stop() flips the shared state that the worker's stop_token observes.
int main() {
    std::atomic<bool> done{false};

    std::jthread worker([&done](std::stop_token st) {
        while (!st.stop_requested()) {
            // ... work in chunks, re-checking the token each time around ...
        }
        done.store(true);
    });

    // The token side just reads the shared state. stop_possible() is true because
    // a source exists that could request a stop; nothing has yet.
    std::stop_token tok = worker.get_stop_token();
    std::cout << std::boolalpha;
    std::cout << "stop possible          = " << tok.stop_possible() << '\n';   // true
    std::cout << "stop requested (before) = " << tok.stop_requested() << '\n'; // false

    // Ask the worker to stop now. request_stop() returns true if THIS call is the
    // one that made the request (it is idempotent: a later call returns false).
    bool made_request = worker.request_stop();
    std::cout << "request_stop returned   = " << made_request << '\n';         // true

    worker.join();   // also happens in the destructor; explicit here to order the output

    std::cout << "stop requested (after)  = " << tok.stop_requested() << '\n'; // true
    std::cout << "worker finished         = " << done.load() << '\n';         // true
    return 0;
}
