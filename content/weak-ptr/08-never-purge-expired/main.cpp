#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

struct Client {
    int id;
    explicit Client(int i) : id(i) {}
};

// A registry of observers held weakly. The objects come and go, but if we never
// remove the expired weak_ptrs the vector keeps growing: each dead entry still
// holds a control block alive (and with make_shared, the object's storage too).
// A long-running server leaks slowly this way.
int main() {
    std::vector<std::weak_ptr<Client>> registry;

    // Many short-lived clients register and then leave.
    for (int i = 0; i < 5; ++i) {
        auto c = std::make_shared<Client>(i);
        registry.push_back(c);
    }   // each Client is destroyed at the end of its iteration, but the weak entry stays

    // --- the anti-pattern: dead weight accumulates ---
    std::size_t expired = 0;
    for (const std::weak_ptr<Client>& w : registry) {
        if (w.expired()) {
            ++expired;
        }
    }
    std::cout << "registry size = " << registry.size()
              << ", expired entries = " << expired << '\n';   // 5, 5: all dead but retained

    // --- the fix: purge expired entries (here, all of them) ---
    registry.erase(
        std::remove_if(registry.begin(), registry.end(),
                       [](const std::weak_ptr<Client>& w) { return w.expired(); }),
        registry.end());
    std::cout << "after purge, registry size = " << registry.size() << '\n';   // 0
    return 0;
}
