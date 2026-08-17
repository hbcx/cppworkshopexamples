#include <atomic>
#include <iostream>
#include <thread>

// Anti-pattern: using relaxed to publish data through a flag.
//
// relaxed makes the flag itself atomic, but it creates NO synchronizes-with edge, so
// it does not publish the data you wrote before setting it. A consumer can see the flag
// true and still read a stale or never-written payload -- there is no happens-before:
//
//   int payload = 0;  std::atomic<bool> ready{false};
//   producer: payload = 99; ready.store(true, relaxed);    // no release side
//   consumer: while(!ready.load(relaxed)){}  use(payload);  // may see payload == 0
//   // On weakly-ordered hardware this really happens. We do not run the racy version:
//   // it is a data race on payload -- undefined behaviour.
//
// The fix is the matching pair: release on the store, acquire on the load. Then the
// write to payload happens-before the read, and the plain data is published safely.
int main() {
    int payload = 0;
    std::atomic<bool> ready{false};

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {   // acquire, not relaxed
        }
        std::cout << "consumer read payload = " << payload << '\n';   // 99
    });

    payload = 99;
    ready.store(true, std::memory_order_release);          // release, not relaxed
    return 0;
}
