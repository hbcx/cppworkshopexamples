#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// You want an atomic counter INSIDE a type, but the type must stay copyable and
// trivial -- a std::atomic member would make it non-copyable and change its layout.
// atomic_ref is the way out: the field stays a plain int, so Metrics remains an
// ordinary aggregate you can copy, memcpy, or serialize, and threads update the
// field atomically through an atomic_ref.
struct Metrics {
    int requests = 0;                   // plain int -- Metrics stays copyable
    int errors = 0;
};

int main() {
    Metrics m;

    std::vector<std::jthread> workers;
    for (int t = 0; t < 6; ++t) {
        workers.emplace_back([&] {
            std::atomic_ref<int> req{m.requests};        // atomic view of the field
            for (int k = 0; k < 500; ++k) {
                req.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    workers.clear();                    // join all

    Metrics snapshot = m;               // still trivially copyable -- that is the point
    std::cout << "requests = " << snapshot.requests
              << ", errors = " << snapshot.errors << '\n';   // requests = 3000, errors = 0
    return 0;
}
