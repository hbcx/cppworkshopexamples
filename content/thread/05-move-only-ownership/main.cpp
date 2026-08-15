#include <atomic>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

int main() {
    // A std::thread OWNS one running thread of execution. That ownership cannot be
    // duplicated -- there is a single underlying OS thread -- so std::thread is
    // MOVE-ONLY: no copy constructor, only a move that TRANSFERS ownership and
    // leaves the source empty (not joinable).
    std::atomic<int> sum{0};
    std::thread t([&] { sum.fetch_add(10); });

    std::thread t2 = std::move(t);   // ownership moves t -> t2; t is now empty
    //   std::thread copy = t2;      // will NOT compile: std::thread has no copy
    //   t.join();                   // would throw std::system_error: t owns nothing

    t2.join();                       // join through the new owner

    // Because it is movable, std::thread lives happily in containers: emplace_back
    // constructs each thread in place, and the vector moves them as it grows.
    std::vector<std::thread> pool;
    for (int i = 0; i < 3; ++i)
        pool.emplace_back([&] { sum.fetch_add(1); });
    for (auto& th : pool) th.join();   // join every thread before the vector dies

    std::cout << "sum = " << sum.load() << '\n';   // 10 + 3 = 13
    return 0;
}
