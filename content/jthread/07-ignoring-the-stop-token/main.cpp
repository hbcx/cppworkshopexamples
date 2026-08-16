#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// Anti-pattern: taking a stop_token but never checking it.
//
// jthread hands the worker a stop_token and auto-calls request_stop() on
// destruction, but that only sets a flag -- the worker has to LOOK at it. A loop
// that never calls stop_requested() cannot be cancelled, so the auto-cancellation
// is silently useless and, if the loop is unbounded, the destructor's join blocks
// forever:
//
//   std::jthread w([](std::stop_token st){ while (true) { /* never reads st */ } });
//   w.request_stop();   // no effect -- nothing checks st
//   // ~jthread calls join()... and HANGS FOREVER. We do not run that version.
//
// The bounded stand-in below finishes on its own, so we can show the mistake:
// the stop is requested but the worker ignores it and runs to its natural end.

int main() {
    std::atomic<bool> ran_to_end{false};

    {
        std::jthread worker([&ran_to_end](std::stop_token /*st ignored!*/) {
            long acc = 0;
            for (int i = 0; i < 1000; ++i) acc += i;   // never checks the token
            (void) acc;
            ran_to_end.store(true);
        });
        worker.request_stop();   // asked to stop, but the loop does not look
    }

    std::cout << std::boolalpha;
    std::cout << "ignored the stop, ran to the end = " << ran_to_end.load()
              << '\n';                                  // true (cancellation had no effect)

    // FIX: check the token so a stop actually ends the work.
    std::atomic<bool> observed_stop{false};
    {
        std::jthread fixed([&observed_stop](std::stop_token st) {
            while (!st.stop_requested()) {
                // ... a chunk of work, then re-check ...
            }
            observed_stop.store(true);
        });
        fixed.request_stop();
    }
    std::cout << "fixed worker observed the stop  = " << observed_stop.load()
              << '\n';                                  // true
    return 0;
}
