#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: mixing atomic_ref access with plain access to the same object.
//
// atomic_ref makes an access atomic only if it GOES THROUGH the atomic_ref. While an
// object is shared, EVERY concurrent access to it must go through an atomic_ref -- a
// plain read or write racing an atomic_ref update is a data race and undefined
// behaviour, even though one side "looks atomic":
//
//   int x = 0;
//   thread A: std::atomic_ref<int>{x}.fetch_add(1);   // atomic
//   thread B: int y = x;                              // PLAIN read -- data race with A
//   // We do not run that racy version.
//
// The fix is to route EVERY concurrent access through atomic_ref for as long as the
// object is shared. Below both the updates and the read-back go through atomic_ref;
// the only plain read happens after all workers have joined.
int main() {
    int shared = 0;

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            std::atomic_ref<int> ref{shared};
            for (int k = 0; k < 1000; ++k)
                ref.fetch_add(1, std::memory_order_relaxed);
        });
    }
    workers.clear();                    // join: after this, no thread touches shared

    // A plain read is fine ONLY here -- every worker has joined and no atomic_ref is
    // live, so there is no concurrent access to race with.
    std::cout << "all access via atomic_ref while shared, counter = " << shared << '\n';  // 8000
    return 0;
}
