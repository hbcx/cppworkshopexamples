#include <iostream>
#include <latch>
#include <thread>
#include <vector>

// arrive_and_wait() is count_down() and wait() in one call: "I am done AND I will
// wait for everyone else." Give every worker its own count and have each one
// arrive_and_wait(), and you get a one-time RENDEZVOUS -- no thread passes the
// line until all have reached it. Here each worker produces its part, rendezvous,
// and only then reads everyone's data, which is now guaranteed complete.
int main() {
    const int N = 3;
    std::latch rendezvous(N);

    std::vector<int> parts(N, 0);       // each worker fills its own entry
    std::vector<int> seen_total(N, 0);  // what each worker sees after the meet
    std::vector<std::jthread> workers;
    for (int i = 0; i < N; ++i) {
        workers.emplace_back([&, i] {
            parts[i] = (i + 1) * 100;       // produce my part
            rendezvous.arrive_and_wait();   // count_down() then wait(): meet here
            // Past the rendezvous, every part is written and visible.
            int t = 0;
            for (int v : parts) t += v;
            seen_total[i] = t;
        });
    }

    workers.clear();                    // join all

    bool all_agree = true;
    for (int t : seen_total) all_agree = all_agree && (t == 600);
    std::cout << "each worker saw the full total (600) = " << std::boolalpha
              << all_agree << '\n';                 // true
    return 0;
}
