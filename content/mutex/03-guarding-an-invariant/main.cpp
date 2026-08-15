#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // Why a mutex and not an atomic: an atomic makes ONE variable's operations
    // indivisible, but many invariants span SEVERAL variables at once. Here two
    // accounts must always sum to 100. A transfer subtracts from one and adds to the
    // other, and those two writes must land together, with no other thread seeing
    // the half-done state (money removed but not yet added). Two separate atomics
    // could be caught mid-transfer; a mutex protects the whole critical section, so
    // the pair changes as a unit.
    int a = 100, b = 0;               // invariant: a + b == 100 at all times
    std::mutex m;

    std::vector<std::thread> pool;
    for (int i = 0; i < 4; ++i)
        pool.emplace_back([&, i] {
            for (int k = 0; k < 100000; ++k) {
                std::lock_guard<std::mutex> guard(m);
                int amount = (k % 7) + 1;
                if (i % 2 == 0) { a -= amount; b += amount; }   // move a -> b
                else            { b -= amount; a += amount; }   // move b -> a
                // Both writes are in ONE critical section, so no other thread can
                // observe a + b as anything but 100.
            }
        });
    for (auto& t : pool) t.join();

    std::cout << "a + b = " << (a + b) << '\n';   // always 100
    return 0;
}
