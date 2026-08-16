#include <future>
#include <iostream>
#include <stop_token>
#include <thread>

// Anti-pattern: calling detach() on a jthread.
//
// detach() severs the jthread from its thread: after it, joinable() is false, so
// the destructor does NOT auto-call request_stop() and does NOT join. You throw
// away both of jthread's features at once and are back to a raw detached thread,
// with its dangling and lifetime hazards -- and you must hand-roll the
// synchronization jthread gave you for free.
//
//   std::jthread w([]{ /* long work touching outer state */ });
//   w.detach();   // no auto-stop, no auto-join; if it outlives what it uses -> UB
//
// Below: after detach we can no longer join, so we must add a promise/future by
// hand just to know the work is done -- exactly the chore jthread removed.

int main() {
    std::cout << std::boolalpha;

    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::jthread worker([&p] { p.set_value(123); });   // no stop_token: detached soon
    worker.detach();

    std::cout << "after detach, joinable = " << worker.joinable() << '\n';  // false
    // No join() is possible now; we must wait on the future ourselves to be sure
    // the detached thread finished before we use its result / exit.
    std::cout << "had to hand-roll a wait, result = " << f.get() << '\n';   // 123

    // FIX: do not detach. A normal jthread auto-joins (and would auto-request_stop
    // for a cancellable worker), so no manual synchronization is needed.
    int result = 0;
    {
        std::jthread fixed([&result] { result = 456; });
    }   // auto-joined here
    std::cout << "fixed jthread auto-joined, result = " << result << '\n';  // 456
    return 0;
}
