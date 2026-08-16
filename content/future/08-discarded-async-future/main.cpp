#include <atomic>
#include <future>
#include <iostream>
#include <vector>

// Anti-pattern: calling std::async but not keeping the returned future.
//
// The future returned by std::async(launch::async, ...) is special: its
// DESTRUCTOR blocks until the task finishes. If you do not store the future, the
// temporary is destroyed at the end of the statement -- so each call waits for
// its own task to complete before the next line runs. A loop of "fire and
// forget" async calls therefore runs strictly one after another, with zero
// parallelism, even though every call looks like it starts a background thread.

std::atomic<int> finished{0};           // how many tasks have completed so far

long work(int) { return 1; }

int main() {
    const int N = 4;

    // --- the trap: discarded futures serialize (this is well-defined, so we run
    //     it) ---
    // Each task records how many tasks had ALREADY finished when it started. If
    // the calls truly overlapped, these would vary run to run. Instead they come
    // out 0,1,2,3: task i starts only after tasks 0..i-1 have fully finished,
    // because the discarded future's destructor blocked at the end of each line.
    std::vector<int> started_after(N, -1);
    for (int i = 0; i < N; ++i) {
        // The (void) cast only silences the [[nodiscard]] on std::async -- we
        // are deliberately not keeping the future, which is exactly the bug. The
        // temporary future is still destroyed at the end of this statement, and
        // that destructor is what blocks until the task finishes.
        (void) std::async(std::launch::async, [i, &started_after] {
            started_after[i] = finished.load();
            work(i);
            finished.fetch_add(1);
        });                              // <-- future destroyed HERE, blocks
    }
    std::cout << "discarded futures, start order = ";
    for (int v : started_after) std::cout << v << ' ';
    std::cout << "(strictly serial)\n";

    // --- the fix: keep the futures, launch all, THEN collect ---
    // Nothing blocks until we call get(), so all N tasks are in flight together.
    std::vector<std::future<long>> futs;
    for (int i = 0; i < N; ++i)
        futs.push_back(std::async(std::launch::async, work, i));
    long total = 0;
    for (auto& f : futs) total += f.get();
    std::cout << "kept futures, results collected = " << total
              << " (ran concurrently)\n";                       // 4
    return 0;
}
