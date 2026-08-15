#include <atomic>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

int main() {
    // std::shared_mutex (C++17) supports two kinds of locking:
    //   * MANY readers at once  -> std::shared_lock  (a shared / read lock)
    //   * ONE writer, exclusive -> std::unique_lock  (an exclusive / write lock)
    // When reads vastly outnumber writes -- a config, a routing table, a cache --
    // this lets all the readers proceed in parallel instead of serializing on a
    // plain mutex, while a write still gets the data all to itself.
    std::shared_mutex sm;
    int config = 1;

    // One writer bumps the value a few times, each under an exclusive lock.
    std::thread writer([&] {
        for (int k = 0; k < 5; ++k) {
            std::unique_lock<std::shared_mutex> w(sm);   // exclusive: blocks readers
            ++config;
        }
    });

    // Several readers read concurrently under shared locks. Each sees one of the
    // valid states (1..6), never a torn value, so we only check the value is in
    // range rather than printing a racy exact number.
    std::atomic<bool> all_in_range{true};
    std::vector<std::thread> readers;
    for (int i = 0; i < 4; ++i)
        readers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k) {
                std::shared_lock<std::shared_mutex> r(sm);   // shared: many at once
                int seen = config;
                if (seen < 1 || seen > 6) all_in_range.store(false);
            }
        });

    writer.join();
    for (auto& t : readers) t.join();

    std::cout << "final config = " << config << '\n';                  // 6
    std::cout << "all reads saw a valid value: " << std::boolalpha
              << all_in_range.load() << '\n';                          // true
    return 0;
}
