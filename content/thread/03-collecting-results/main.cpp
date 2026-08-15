#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

int main() {
    // A thread's callable cannot RETURN a value to you -- its return is discarded,
    // and std::future (the tool that does capture a return) is a later topic. The
    // plain, race-free way to collect results is to give each worker its OWN slot
    // to write. No two threads touch the same element, so there is no data race and
    // no locking; you join them all and then read the slots.
    const int N = 4;
    std::vector<long> partial(N, 0);          // one result slot per worker
    std::vector<std::thread> workers;

    for (int i = 0; i < N; ++i)
        workers.emplace_back([i, &partial] {
            long sum = 0;
            for (int k = 1; k <= 100; ++k)
                sum += static_cast<long>(i) * 100 + k;   // some per-worker work
            partial[i] = sum;                            // write only MY slot
        });

    for (auto& t : workers) t.join();         // every write is visible after join

    long total = std::accumulate(partial.begin(), partial.end(), 0L);
    std::cout << "total from " << N << " workers = " << total << '\n';   // 80200
    return 0;
}
