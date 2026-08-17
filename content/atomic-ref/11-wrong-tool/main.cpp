#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: reaching for atomic_ref when you own the type and could just declare
// it std::atomic.
//
// atomic_ref exists for the case where you CANNOT change the object's type: a plain
// array you must hand to an int*-taking API, a field in a type that must stay
// trivially copyable, or data from a header you do not control. When you DO own the
// declaration, std::atomic<int> is clearer and safer:
//   * the atomicity is part of the type, so a plain access is impossible -- the
//     mixing bug cannot happen;
//   * there is no alignment precondition to remember;
//   * there is no atomic_ref lifetime to keep in scope.
//
// Sprinkling atomic_ref over an int you fully control just moves the guarantee from
// the type to every call site, where one forgotten plain access is undefined
// behaviour. Below the counter is simply a std::atomic<int> -- the guarantee is
// built in, once.
int main() {
    std::atomic<int> counter{0};        // atomicity is part of the type, everywhere

    std::vector<std::jthread> workers;
    for (int t = 0; t < 8; ++t) {
        workers.emplace_back([&] {
            for (int k = 0; k < 1000; ++k)
                counter.fetch_add(1, std::memory_order_relaxed);
        });
    }
    workers.clear();                    // join all

    std::cout << "owned type -> std::atomic, counter = " << counter.load() << '\n';   // 8000
    return 0;
}
