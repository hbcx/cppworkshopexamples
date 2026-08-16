#include <chrono>
#include <future>
#include <iostream>
#include <thread>

// std::async takes a launch policy that decides WHERE and WHEN the task runs:
//   std::launch::async    -> run now, on a new thread (real concurrency)
//   std::launch::deferred -> do not run yet; run lazily on the calling thread
//                            the first time you call get() or wait()
// The two behave very differently, and the DEFAULT (no policy) is "async or
// deferred, implementation's choice" -- so you cannot assume either one. This
// example makes the difference visible without relying on timing.

int main() {
    const std::thread::id main_id = std::this_thread::get_id();

    // --- deferred: nothing runs until we ask for the result ---
    std::future<std::thread::id> lazy =
        std::async(std::launch::deferred,
                   [] { return std::this_thread::get_id(); });

    // A deferred task has not started. wait_for reports exactly that, with no
    // real waiting, so we can observe the state deterministically.
    bool not_started = (lazy.wait_for(std::chrono::seconds(0)) ==
                        std::future_status::deferred);
    std::cout << "deferred, before get: not started yet = "
              << std::boolalpha << not_started << '\n';       // true

    // get() runs the task now, ON THIS THREAD, so its id equals main's.
    bool ran_on_caller = (lazy.get() == main_id);
    std::cout << "deferred ran on the calling thread       = "
              << ran_on_caller << '\n';                       // true

    // --- async: runs immediately on a separate thread ---
    std::future<std::thread::id> eager =
        std::async(std::launch::async,
                   [] { return std::this_thread::get_id(); });
    bool ran_on_other = (eager.get() != main_id);
    std::cout << "async ran on a different thread          = "
              << ran_on_other << '\n';                        // true
    return 0;
}
