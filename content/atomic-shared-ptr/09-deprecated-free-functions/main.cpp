#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// Anti-pattern: using the deprecated std::atomic_load / std::atomic_store free
// functions on a shared_ptr.
//
// Before C++20 the only way to touch a shared_ptr atomically was a family of free
// functions taking the ADDRESS of the shared_ptr:
//
//   std::shared_ptr<int> sp = ...;
//   auto snap = std::atomic_load(&sp);                    // DEPRECATED in C++20
//   std::atomic_store(&sp, std::make_shared<int>(2));     // DEPRECATED in C++20
//
// They are error-prone -- any plain access to sp elsewhere silently breaks atomicity,
// just like mixing plain access with atomic_ref -- and they are deprecated as of
// C++20, to be removed in a later standard. We do not call them (they would trip
// -Wdeprecated under -Werror anyway).
//
// The fix is the std::atomic<std::shared_ptr<T>> specialization: atomicity lives in
// the type, so no call site can forget it. Below is the modern equivalent.
int main() {
    std::atomic<std::shared_ptr<int>> sp{std::make_shared<int>(1)};

    std::jthread writer([&] {
        sp.store(std::make_shared<int>(2));     // was: std::atomic_store(&sp, ...)
    });

    std::vector<int> seen(3, -1);
    std::vector<std::jthread> readers;
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back([&, i] {
            std::shared_ptr<int> snap = sp.load();   // was: std::atomic_load(&sp)
            seen[i] = *snap;
        });
    }
    readers.clear();
    writer.join();

    bool valid = true;
    for (int v : seen) valid = valid && (v == 1 || v == 2);
    std::cout << "modern atomic<shared_ptr> replaces the deprecated free functions = "
              << std::boolalpha << valid << '\n';   // true
    return 0;
}
