#include <atomic>
#include <iostream>
#include <thread>

int main() {
    // C++20's std::jthread is a std::thread that JOINS ITSELF in its destructor.
    // That removes the single biggest std::thread footgun: forgetting to join, or
    // an exception skipping the join, either of which makes a plain std::thread
    // call std::terminate. jthread is simply RAII applied to a thread.
    std::atomic<int> done{0};
    {
        std::jthread worker([&] { done.store(7); });
        // No explicit join. When 'worker' leaves this scope, its destructor joins
        // automatically -- even if the scope is left early by an exception.
    }   // <-- worker joined here, for free
    std::cout << "jthread finished, done = " << done.load() << '\n';   // 7

    // jthread also carries cooperative cancellation: it hands the callable a
    // std::stop_token, and other code can call request_stop() so the worker can
    // check stop_requested() and bow out early. That mechanism is a topic of its
    // own; the headline here is the auto-join.
    //
    // Guidance: prefer std::jthread so a thread can never be left unjoined. Reach
    // for std::thread only when you specifically need detach or manual control over
    // exactly where the join happens.
    return 0;
}
