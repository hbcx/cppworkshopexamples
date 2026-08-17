#include <atomic>
#include <barrier>
#include <iostream>
#include <thread>
#include <vector>

// A std::barrier can run a COMPLETION FUNCTION once per phase: when the last
// thread arrives, the barrier runs your function on that one thread -- while all
// the others are still blocked -- and only then releases everyone. It is the
// perfect place to combine the phase's results and set up the next phase, because
// it runs exactly once, alone, with no other worker touching the shared state.
int main() {
    const int N = 3;
    std::atomic<long> phase_sum{0};     // workers add into this each phase
    std::vector<long> phase_results;    // written ONLY by the completion function

    // noexcept is required. It runs single-threaded per phase, so touching
    // phase_results without a lock is safe.
    auto on_phase_end = [&]() noexcept {
        phase_results.push_back(phase_sum.load());
        phase_sum.store(0);             // reset for the next phase
    };

    std::barrier sync(N, on_phase_end);
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            for (int phase = 1; phase <= 3; ++phase) {
                phase_sum.fetch_add(static_cast<long>(i + 1) * phase);
                sync.arrive_and_wait();     // completion aggregates when all arrive
            }
        });
    }

    workers.clear();                    // join all

    std::cout << "per-phase sums:";
    for (long r : phase_results) std::cout << ' ' << r;   // 6 12 18
    std::cout << '\n';
    return 0;
}
