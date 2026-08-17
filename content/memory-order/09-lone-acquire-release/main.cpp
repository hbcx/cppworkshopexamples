#include <atomic>
#include <iostream>
#include <thread>

// Anti-pattern: an acquire (or release) with no matching partner.
//
// Ordering comes from a PAIR: a release store synchronizes-with the acquire load that
// reads the value it wrote. An acquire load is meaningless on its own -- if the value it
// reads was written by a RELAXED store, there is no release to pair with, so no
// happens-before is created and the data is still unpublished:
//
//   producer: payload = 99; ready.store(true, relaxed);    // relaxed: no release side
//   consumer: while(!ready.load(acquire)){}  use(payload);  // acquire pairs with nothing
//   // The acquire LOOKS careful but synchronizes with no release -> still a data race.
//   // We do not run that version.
//
// Both sides must carry the order: release on the store AND acquire on the load. Fixing
// only one side fixes nothing. Below both are present, so the pair synchronizes.
int main() {
    int payload = 0;
    std::atomic<bool> ready{false};

    std::jthread consumer([&] {
        while (!ready.load(std::memory_order_acquire)) {   // the acquire half...
        }
        std::cout << "consumer read payload = " << payload << '\n';   // 99
    });

    payload = 99;
    ready.store(true, std::memory_order_release);          // ...and the release half it pairs with
    return 0;
}
