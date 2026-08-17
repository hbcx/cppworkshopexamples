#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// Anti-pattern: sharing a plain std::shared_ptr across threads that read and write it.
//
// A common myth is that shared_ptr is "thread-safe". Only its REFERENCE COUNT is
// atomic -- so each thread copying a shared_ptr is fine. The shared_ptr OBJECT (its
// object pointer plus control-block pointer) is not atomic, so if one thread reassigns
// a shared_ptr while another reads or copies that SAME instance, it is a data race:
//
//   std::shared_ptr<int> sp = ...;
//   thread A: sp = std::make_shared<int>(2);   // writes the two internal pointers
//   thread B: auto local = sp;                 // reads them at the same time -> race
//   // We do not run that racy version.
//
// The fix is std::atomic<std::shared_ptr<T>>: load() and store() make the whole handle
// update atomically. Below every access to the shared handle goes through the atomic
// wrapper, so the readers' copies are always well-formed.
int main() {
    std::atomic<std::shared_ptr<int>> shared{std::make_shared<int>(1)};

    std::jthread writer([&] {
        for (int k = 0; k < 100; ++k)
            shared.store(std::make_shared<int>(k));
    });

    std::vector<int> ok(4, 0);
    std::vector<std::jthread> readers;
    for (int i = 0; i < 4; ++i) {
        readers.emplace_back([&, i] {
            for (int k = 0; k < 100; ++k) {
                std::shared_ptr<int> snap = shared.load();   // atomic, never torn
                if (snap) ok[i] = 1;
            }
        });
    }
    readers.clear();
    writer.join();

    bool all_read_safely = true;
    for (int v : ok) all_read_safely = all_read_safely && v == 1;
    std::cout << "all readers loaded a valid pointer through the atomic = "
              << std::boolalpha << all_read_safely << '\n';   // true
    return 0;
}
