#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// Anti-pattern: expecting request_stop() to forcibly kill the thread.
//
// request_stop() is not preemption. It sets a flag; it does not interrupt a
// running computation, and it does not break a thread out of a blocking system
// call. A worker that does not CHECK the token between units of work -- or that is
// stuck in a long blocking read -- keeps going to the end regardless. There is no
// safe forced-kill in C++, because it would leave locks held and objects
// half-destroyed.
long long sum_to(long long n) {
    long long total = 0;
    for (long long i = 1; i <= n; ++i) total += i;   // never checks any stop_token
    return total;
}

int main() {
    std::atomic<long long> result{0};

    {
        std::jthread worker([&result](std::stop_token /*st*/) {
            // A long computation that does not poll the token. request_stop()
            // below cannot cut it short: it runs to completion.
            result.store(sum_to(1000000));
        });
        worker.request_stop();   // sets the flag -- but nothing checks it
    }

    std::cout << "result after request_stop = " << result.load()
              << " (full sum, not interrupted)\n";        // 500000500000 (1..1000000)

    // FIX: to actually stop, the work must check the token at safe points (a loop
    // that breaks on stop_requested), or use a stop_callback to unblock a blocking
    // call. request_stop only ever ASKS.
    std::atomic<bool> observed_stop{false};
    {
        std::jthread cancellable([&observed_stop](std::stop_token st) {
            while (!st.stop_requested()) {
                // ... work in chunks, re-checking the token each pass ...
            }
            observed_stop.store(true);
        });
        cancellable.request_stop();
    }
    std::cout << "token-checking worker stopped = " << std::boolalpha
              << observed_stop.load() << '\n';             // true
    return 0;
}
