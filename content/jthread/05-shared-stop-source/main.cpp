#include <atomic>
#include <iostream>
#include <stop_token>
#include <thread>
#include <vector>

// One std::stop_source can cancel MANY workers at once. Because every token
// copied from a source shares the same state, handing the same token (or tokens
// from the same source) to a group of threads means a single request_stop()
// stops all of them together -- a fan-out cancel. This is the shape behind
// "first result wins", or shutting a whole worker pool down on exit.
int main() {
    std::stop_source src;
    std::atomic<int> cancelled{0};
    const int N = 5;

    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        // Each worker watches the SHARED token, captured by value (a token is a
        // cheap handle; all copies see the same source).
        workers.emplace_back([&cancelled, st = src.get_token()] {
            while (!st.stop_requested()) {
                // ... work, re-checking the shared token each pass ...
            }
            cancelled.fetch_add(1);
        });
    }

    src.request_stop();     // ONE call cancels every worker
    workers.clear();        // destroying each jthread joins it

    std::cout << "workers cancelled by one stop_source = " << cancelled.load()
              << " of " << N << '\n';                    // 5 of 5
    return 0;
}
