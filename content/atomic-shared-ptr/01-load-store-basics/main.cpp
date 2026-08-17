#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// A plain std::shared_ptr has an atomic REFERENCE COUNT, but the shared_ptr object
// itself -- its pair of pointers -- is not atomic: two threads that load and store
// the SAME shared_ptr instance concurrently race. std::atomic<std::shared_ptr<T>>
// (C++20, header memory) fixes that. load() returns a full owning copy that stays
// safe to use even if another thread replaces the pointer a moment later, and store()
// swaps in a new pointer atomically, dropping the old one's reference for you.
int main() {
    std::atomic<std::shared_ptr<int>> current{std::make_shared<int>(0)};

    // One writer publishes a new value; several readers take a safe snapshot.
    std::jthread writer([&] {
        current.store(std::make_shared<int>(42));
    });

    std::vector<int> seen(4, -1);
    std::vector<std::jthread> readers;
    for (int i = 0; i < 4; ++i) {
        readers.emplace_back([&, i] {
            std::shared_ptr<int> snap = current.load();   // a safe owning copy
            seen[i] = *snap;                              // 0 or 42, never a torn read
        });
    }
    readers.clear();                    // join readers
    writer.join();

    // Each reader saw either the old (0) or the new (42) value -- always a valid one,
    // never a half-written pointer.
    bool all_valid = true;
    for (int v : seen) all_valid = all_valid && (v == 0 || v == 42);
    std::cout << "every reader saw a valid published value = " << std::boolalpha << all_valid << '\n';  // true
    return 0;
}
