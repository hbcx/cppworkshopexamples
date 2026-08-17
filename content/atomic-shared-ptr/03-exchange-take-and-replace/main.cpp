#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// exchange() atomically installs a new pointer AND returns the old one in a single
// step -- no window where the slot is empty or owned twice. It is the tool for "take
// whatever is there now and put something fresh in its place": rotating a buffer,
// draining a batch, claiming the current item. A useful property falls out of it:
// each value is handed to exactly one taker. Here N workers each swap in their own
// new value and take out whatever was there.
int main() {
    const int N = 8;
    std::atomic<std::shared_ptr<int>> slot{std::make_shared<int>(0)};

    std::vector<int> recovered(N, -1);
    std::vector<std::jthread> workers;
    for (int i = 1; i <= N; ++i) {
        workers.emplace_back([&, i] {
            std::shared_ptr<int> old = slot.exchange(std::make_shared<int>(i));
            recovered[i - 1] = *old;    // the value this thread took out
        });
    }
    workers.clear();                    // join all

    // Values 0..N were each placed once and taken out exactly once: the N recovered
    // values plus whatever is left in the slot sum to 0+1+...+N, whatever the order.
    long sum = *slot.load();
    for (int v : recovered) sum += v;
    std::cout << "each value handed off exactly once, sum = " << sum
              << " (expected " << (N * (N + 1) / 2) << ")\n";   // 36
    return 0;
}
