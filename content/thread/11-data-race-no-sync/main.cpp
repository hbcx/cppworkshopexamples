#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: two or more threads reading and writing the same plain variable
    // with no synchronization. ++counter on a shared int is really load-add-store;
    // interleaved, two threads read the same old value and both write back the same
    // new one, silently losing updates. It is a DATA RACE -- undefined behaviour,
    // even when the total "looks close" on a lucky run.
    //
    //   long counter = 0;                          // plain, shared
    //   auto work = [&]{ for (...) ++counter; };   // UNSYNCHRONIZED write -> race
    //
    // We do NOT run that version: it is undefined behaviour, its total is
    // nondeterministic, and ThreadSanitizer (which the CI runs) would rightly abort
    // the build with a data-race report. That report is the tool doing its job.

    // FIX: make the shared counter a std::atomic (or guard it with a std::mutex).
    // fetch_add is a single indivisible read-modify-write, so no update is lost and
    // the total is exact and deterministic.
    std::atomic<long> counter{0};
    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&] {
            for (int k = 0; k < 100000; ++k) counter.fetch_add(1);
        });
    for (auto& t : pool) t.join();

    std::cout << "synchronized total = " << counter.load() << '\n';   // 400000

    // Threads make sharing the default hazard, not the exception: any data reached
    // from more than one thread where at least one writes must be synchronized --
    // with an atomic, a mutex, or by not sharing it (give each thread its own slot).
    return 0;
}
