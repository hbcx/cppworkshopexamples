#include <atomic>
#include <iostream>
#include <thread>

int main() {
    // ANTI-PATTERN: letting a std::thread be destroyed while it is still joinable.
    // If a std::thread's destructor runs while it still owns a thread that was never
    // joined or detached, it calls std::terminate() and aborts the whole program --
    // not an exception you can catch, a hard stop.
    //
    //   {
    //       std::thread t([]{ /* work */ });
    //   }   // t destroyed here, never joined or detached -> std::terminate()
    //
    // We do NOT run that: it would abort the process, and abort is not a result the
    // build gate can capture. The lesson is that the destructor does NOT quietly
    // join for you -- forgetting the join is fatal, not just a leak.

    // FIX: join (or detach) before the thread object goes out of scope.
    std::atomic<int> ran{0};
    {
        std::thread t([&] { ran.fetch_add(1); });
        t.join();                    // t is now non-joinable -> safe to destroy
    }
    std::cout << "after join, worker ran " << ran.load() << " time(s)\n";   // 1

    // Better still, use std::jthread (C++20, see example 07): its destructor joins
    // automatically, so a thread can never be left joinable -- the mistake becomes
    // impossible, even if an exception skips your join.
    return 0;
}
