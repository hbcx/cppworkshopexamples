#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// compare_exchange_strong makes a SINGLE attempt: it succeeds only if the value still
// equals `expected`, and -- unlike the weak form -- it never fails SPURIOUSLY. That
// makes it the right choice when you are NOT in a retry loop, where one clean
// yes-or-no answer is what you want. Here many threads race to claim a slot exactly
// once: each tries to move it from 0 (unclaimed) to its own id, and only one can win.
int main() {
    std::atomic<int> owner{0};          // 0 = unclaimed
    std::atomic<int> winners{0};

    std::vector<std::thread> threads;
    for (int i = 1; i <= 8; ++i) {
        threads.emplace_back([&, i] {
            int expected = 0;
            // One attempt, no loop. strong so a spurious failure cannot make us
            // wrongly believe the slot was already taken and give up.
            if (owner.compare_exchange_strong(expected, i)) {
                winners.fetch_add(1);   // this thread claimed it
            }
            // On failure, expected now holds the id of whoever won -- we simply lose.
        });
    }
    for (auto& t : threads) t.join();

    // Exactly one CAS succeeds: the slot goes 0 -> winner once, every other thread
    // sees expected != 0 and fails. Deterministic regardless of interleaving.
    std::cout << "exactly one thread claimed the slot = " << std::boolalpha
              << (winners.load() == 1)
              << ", owner in 1..8 = "
              << (owner.load() >= 1 && owner.load() <= 8) << '\n';   // true, true
    return 0;
}
