#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// A common need: many threads update different (and sometimes the same) slots of one
// shared array. Making the array std::atomic<int>[] would change its type and stop
// you handing it to code that wants a plain int* -- serialization, a numeric library,
// a GPU upload. atomic_ref keeps the array a plain std::vector<int> and makes only
// the updates atomic: one atomic_ref per element touched, and no per-slot mutex.
int main() {
    const int BUCKETS = 4;
    std::vector<int> histogram(BUCKETS, 0);      // a plain int array, fixed size

    const int THREADS = 8;
    const int PER_THREAD = 1000;
    std::vector<std::jthread> workers;
    for (int t = 0; t < THREADS; ++t) {
        workers.emplace_back([&, t] {
            for (int k = 0; k < PER_THREAD; ++k) {
                int bucket = (t + k) % BUCKETS;                  // spread the hits
                std::atomic_ref<int> slot{histogram[bucket]};    // atomic view of one slot
                slot.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    workers.clear();                    // join all

    long total = 0;
    for (int c : histogram) total += c;
    std::cout << "counted " << total << " hits across " << BUCKETS << " buckets\n";  // 8000
    return 0;
}
