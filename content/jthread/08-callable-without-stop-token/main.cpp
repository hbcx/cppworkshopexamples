#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// Anti-pattern: giving jthread a callable that does not take a stop_token.
//
// jthread accepts BOTH forms: a callable taking a leading std::stop_token, and
// one taking none. If you write the worker like an old std::thread lambda -- []{...}
// with no token parameter -- it compiles and runs, but the worker never receives
// the token, so request_stop() has no channel to it and cannot cancel it. The
// cancellation looks set up (it is a jthread!) but is not wired to the work.
//
//   std::jthread w([]{ while (true) { /* no token in scope at all */ } });
//   w.request_stop();   // there is nothing here that could observe it
//   // ~jthread join() HANGS. We do not run the unbounded version.

int main() {
    std::atomic<bool> finished{false};

    {
        // Wrong: no stop_token parameter. jthread runs it as a plain task.
        std::jthread worker([&finished] {
            long acc = 0;
            for (int i = 0; i < 1000; ++i) acc += i;   // bounded stand-in
            (void) acc;
            finished.store(true);
        });
        worker.request_stop();   // the worker has no token -> nothing to cancel
    }
    std::cout << std::boolalpha;
    std::cout << "no-token worker uncancellable, finished on its own = "
              << finished.load() << '\n';               // true

    // FIX: accept std::stop_token as the FIRST parameter so jthread passes its
    // token in; now the worker can be cancelled.
    std::atomic<bool> observed_stop{false};
    {
        std::jthread fixed([&observed_stop](std::stop_token st) {
            while (!st.stop_requested()) { }
            observed_stop.store(true);
        });
        fixed.request_stop();
    }
    std::cout << "fixed worker (takes the token) observed stop        = "
              << observed_stop.load() << '\n';          // true
    return 0;
}
