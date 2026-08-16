#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// A std::stop_token is a standalone cancellation handle -- it is not tied to
// jthread. You can create a std::stop_source yourself, hand out its token to any
// function or thread, and cancel from wherever you like. This is how you make a
// deep, ordinary computation cancellable: pass the token in like any parameter
// and check it between chunks of work. Here a plain std::thread runs the work and
// the main thread triggers the stop through the source it owns.
void crunch(std::stop_token st, std::atomic<bool>& finished) {
    while (!st.stop_requested()) {
        // ... process the next chunk, then loop back and re-check the token ...
    }
    finished.store(true);
}

int main() {
    std::stop_source src;                 // we own the shared cancellation state
    std::atomic<bool> finished{false};

    std::thread t(crunch, src.get_token(), std::ref(finished));

    // ... elsewhere -- a timeout, a user pressing Cancel, a shutdown -- decides
    //     to stop the work. One call flips the token crunch is watching.
    src.request_stop();
    t.join();

    std::cout << "cancelled via a standalone stop_source = " << std::boolalpha
              << finished.load() << '\n';    // true
    return 0;
}
