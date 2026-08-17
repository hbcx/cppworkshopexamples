#include <atomic>
#include <barrier>
#include <iostream>
#include <thread>
#include <vector>

// Anti-pattern: doing throwing or slow work in a barrier completion function.
//
// The completion runs on the last arriving thread while every other thread is
// blocked, so it has two hard constraints:
//
//   * It must be noexcept. An exception escaping it calls std::terminate -- the
//     type even requires noexcept, so a throwing body is a contract violation.
//   * It must be quick. The whole group is parked until it returns, so any slow
//     or blocking work there stalls every worker.
//
//   auto bad = [&]() noexcept {
//       auto rows = db.query(...);        // slow AND may throw -> stalls, or terminates
//       phase_results.push_back(rows);
//   };
//   // We do not run that. Keep the completion trivial instead:
//
// The fix: do the heavy or fallible work in the WORKERS' phase; leave the
// completion a tiny, non-throwing publish/reset step.
int main() {
    const int N = 3;
    std::atomic<long> phase_sum{0};
    std::vector<long> results;

    // Trivial, noexcept, no blocking: just publish the phase total and reset.
    auto on_complete = [&]() noexcept {
        results.push_back(phase_sum.load());
        phase_sum.store(0);
    };

    std::barrier sync(N, on_complete);
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            for (int phase = 1; phase <= 3; ++phase) {
                // The real (possibly heavy) work lives HERE, in parallel, not in
                // the completion.
                phase_sum.fetch_add(static_cast<long>(i + 1) * phase);
                sync.arrive_and_wait();
            }
        });
    }
    workers.clear();

    std::cout << "per-phase sums:";
    for (long r : results) std::cout << ' ' << r;   // 6 12 18
    std::cout << '\n';
    return 0;
}
