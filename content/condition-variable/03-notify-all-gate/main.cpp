#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // notify_one wakes ONE waiter; notify_all wakes EVERY thread waiting on the cv.
    // Use notify_all when a single change should release many waiters at once -- a
    // start gate that holds a pool of workers until the go signal, a phase change,
    // a shutdown flag every thread is parked on.
    std::mutex m;
    std::condition_variable cv;
    bool go = false;
    std::atomic<int> started{0};

    std::vector<std::thread> workers;
    for (int i = 0; i < 8; ++i)
        workers.emplace_back([&] {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&] { return go; });   // all 8 block here until 'go'
            lock.unlock();                        // done with shared state
            started.fetch_add(1);                 // record that we passed the gate
        });

    // Open the gate once and wake ALL of them together. A notify_one here would
    // release just one worker and leave the other seven asleep.
    {
        std::lock_guard<std::mutex> lock(m);
        go = true;
    }
    cv.notify_all();

    for (auto& t : workers) t.join();
    std::cout << "workers released = " << started.load() << '\n';   // 8
    return 0;
}
