#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: two mutexes locked in INCONSISTENT order by different threads.
    // If thread 1 does lock(A) then lock(B), and thread 2 does lock(B) then lock(A),
    // an interleaving leaves each holding one and waiting for the other forever --
    // a deadlock: the program hangs, with no crash and no error message.
    //
    //   // thread 1                          // thread 2
    //   std::lock_guard g1(A);               std::lock_guard g1(B);
    //   std::lock_guard g2(B);  // waits      std::lock_guard g2(A);  // waits -> hang
    //
    // We do NOT run that: a real deadlock would hang the build forever.

    std::mutex A, B;
    long moved = 0;

    // FIX: every thread locks the mutexes in the SAME order (A before B), so no
    // cycle of "each waits for the other" can form. (std::scoped_lock(A, B) is the
    // order-proof alternative -- see the scoped_lock example -- which locks both at
    // once and does not care what order you name them.)
    auto work = [&] {
        for (int k = 0; k < 1000; ++k) {
            std::lock_guard<std::mutex> g1(A);
            std::lock_guard<std::mutex> g2(B);      // always A then B, everywhere
            ++moved;
        }
    };

    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i) pool.emplace_back(work);
    for (auto& t : pool) t.join();

    std::cout << "moved = " << moved << '\n';   // 4000, no deadlock
    return 0;
}
