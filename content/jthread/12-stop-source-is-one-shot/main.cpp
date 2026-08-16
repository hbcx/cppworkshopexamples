#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>

// Anti-pattern: reusing a stop_source after it has been stopped.
//
// A stop_source is one-shot. Once request_stop() succeeds it stays in the stopped
// state permanently -- there is no reset. Every token you then take from it is
// born already stopped, so the NEXT worker you hand it to cancels itself
// instantly and does no work. Cancellation state does not recycle: make a fresh
// stop_source for each cancellable operation.
int main() {
    std::cout << std::boolalpha;

    std::stop_source src;
    src.request_stop();     // first operation cancelled; source now permanently stopped

    // Reusing the same source for a second operation: its token is already stopped
    // when the worker starts, so the worker checks once and skips its work.
    std::atomic<bool> did_work{false};
    {
        std::jthread worker([&did_work, st = src.get_token()] {
            if (!st.stop_requested()) did_work.store(true);   // token born stopped -> never runs
        });
    }   // auto-joined
    std::cout << "reused stopped source: worker did any work = "
              << did_work.load() << '\n';         // false -- token was born stopped

    // FIX: a fresh stop_source per operation starts un-stopped, so its token is
    // live and the worker does its work. (Nobody stops 'fresh' here, so the check
    // is deterministic regardless of when the worker runs.)
    std::stop_source fresh;
    std::atomic<bool> did_work2{false};
    {
        std::jthread worker([&did_work2, st = fresh.get_token()] {
            if (!st.stop_requested()) did_work2.store(true);  // fresh token not stopped -> runs
        });
    }   // auto-joined
    std::cout << "fresh source: worker did its work          = "
              << did_work2.load() << '\n';         // true
    return 0;
}
