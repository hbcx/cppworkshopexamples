#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// Anti-pattern: mutating the pointed-to object through an atomic<shared_ptr> and
// expecting that to be synchronized.
//
// atomic<shared_ptr> makes swapping the POINTER atomic. It does NOTHING for the object
// the pointer points at. If several threads load() the same shared_ptr and then write
// through it, they all touch the SAME object with no synchronization -- an ordinary
// data race on the pointee:
//
//   auto p = data.load();
//   p->total++;              // many threads, one object -> race on *p
//   // We do not run that.
//
// The right model is IMMUTABLE snapshots: never modify a published object. To change
// the state, build a NEW object and store() it, so a reader sees the whole old object
// or the whole new one, never a half-written one. Below each update publishes a fresh
// object instead of mutating the live one.
struct State { long total; };

int main() {
    std::atomic<std::shared_ptr<const State>> data{
        std::make_shared<const State>(State{0})};

    // A single writer publishes new immutable snapshots; readers only ever read.
    std::jthread writer([&] {
        for (int k = 1; k <= 1000; ++k) {
            std::shared_ptr<const State> old = data.load();
            data.store(std::make_shared<const State>(State{old->total + 1}));
        }
    });

    std::vector<std::jthread> readers;
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back([&] {
            long seen = 0;
            for (int k = 0; k < 1000; ++k)
                seen = data.load()->total;   // reading an immutable object: safe
            (void) seen;
        });
    }
    readers.clear();
    writer.join();

    // One writer, so the read-then-publish loop loses nothing: the final total is 1000.
    std::cout << "final published total = " << data.load()->total << '\n';   // 1000
    return 0;
}
