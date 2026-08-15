#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // On an atomic you never touch the value directly -- you go through its
    // operations. store() writes, load() reads, both indivisibly.
    std::atomic<int> value{0};
    value.store(10);
    std::cout << "loaded: " << value.load() << '\n';           // 10

    // exchange() atomically SETS a new value and RETURNS the previous one, in one
    // step. It is the basis of claiming a resource: whoever exchanges gets to see
    // what was there before, with no gap for another thread to slip in.
    int previous = value.exchange(20);
    std::cout << "exchange returned old: " << previous
              << ", now: " << value.load() << '\n';            // 10, 20

    // A one-shot claim: many threads try to exchange a flag from false to true;
    // exactly ONE sees the old value false and "wins" the claim. exchange makes
    // that test-and-set race-free.
    std::atomic<bool> claimed{false};
    std::atomic<int> winners{0};
    std::vector<std::thread> pool;
    for (int i = 0; i < 8; ++i)
        pool.emplace_back([&] {
            if (!claimed.exchange(true)) winners.fetch_add(1);   // old was false -> won
        });
    for (auto& t : pool) t.join();

    std::cout << "winners: " << winners.load() << '\n';        // exactly 1
    return 0;
}
