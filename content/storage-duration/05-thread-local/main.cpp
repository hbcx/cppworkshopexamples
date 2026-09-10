#include <iostream>
#include <thread>
#include <vector>

// thread_local storage duration: one independent instance per thread, initialized
// on first use in that thread and destroyed when the thread ends.
thread_local int t_counter = 0;

void work(int increments, int& out) {
    for (int i = 0; i < increments; ++i)
        ++t_counter;      // this thread's OWN counter -- no sharing, so no race
    out = t_counter;
}

int main() {
    std::vector<int> results(3, -1);
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i)
        threads.emplace_back(work, (i + 1) * 5, std::ref(results[i]));
    for (auto& t : threads)
        t.join();

    for (int i = 0; i < 3; ++i)
        std::cout << "thread " << i << " counted to " << results[i] << "\n";
    // The main thread has its own t_counter, untouched by the workers.
    std::cout << "main thread t_counter = " << t_counter << "\n";
    return 0;
}
