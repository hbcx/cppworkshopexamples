#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // hardware_concurrency() is a HINT: how many threads can genuinely run at the
    // same time on this machine (cores / hyperthreads). It is only a hint -- it may
    // return 0 when the number is unknown -- so always guard it. Use it to size a
    // worker pool instead of hard-coding a guess.
    unsigned n = std::thread::hardware_concurrency();
    if (n == 0) n = 4;                 // fall back if the hint is unavailable

    // Split a fixed job across the pool: sum every number from 1 to T. Each worker
    // takes a strided slice (worker w handles w+1, w+1+n, w+1+2n, ...), so together
    // they cover 1..T exactly once with no overlap. The TOTAL is therefore the same
    // no matter how many threads this machine happens to run -- the result is
    // deterministic even though n itself is machine-specific.
    const long T = 6000;
    std::atomic<long> total{0};
    std::vector<std::thread> pool;
    for (unsigned w = 0; w < n; ++w)
        pool.emplace_back([w, n, &total] {
            long local = 0;
            for (long k = w + 1; k <= T; k += n) local += k;   // this worker's slice
            total.fetch_add(local);                            // combine once at end
        });
    for (auto& t : pool) t.join();

    std::cout << "sum 1.." << T << " = " << total.load() << '\n';   // 18003000
    return 0;
}
