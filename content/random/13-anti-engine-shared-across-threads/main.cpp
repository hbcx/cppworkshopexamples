#include <random>
#include <thread>
#include <iostream>
#include <cstdint>

// TRAP (described, not run): sharing ONE engine across threads is a data race.
// An engine is stateful -- each call mutates its internal state -- and none of
// the standard engines are thread-safe. Two threads calling the same engine
// concurrently race on that state and corrupt the sequence:
//     std::mt19937 shared(1);
//     auto work = [&]{ for (...) use(shared()); };   // <-- race on shared
//     std::thread t1(work), t2(work);                //     undefined behaviour
//
// FIX: give each thread its OWN engine, seeded distinctly. No shared state, no
// race, and each stream is independent and reproducible.
static std::uint64_t sum_stream(std::uint32_t seed, int n) {
    std::mt19937 engine(seed);                    // this thread's own engine
    std::uniform_int_distribution<int> d(1, 1000);
    std::uint64_t total = 0;
    for (int i = 0; i < n; ++i) total += static_cast<std::uint64_t>(d(engine));
    return total;
}

int main() {
    std::uint64_t s1 = 0, s2 = 0;
    std::thread t1([&] { s1 = sum_stream(111, 100000); });
    std::thread t2([&] { s2 = sum_stream(222, 100000); });
    t1.join();
    t2.join();

    // Written by one thread each, read after join, printed in a fixed order --
    // deterministic despite the threads.
    std::cout << "thread 1 sum (seed 111) = " << s1 << "\n";
    std::cout << "thread 2 sum (seed 222) = " << s2 << "\n";
}
