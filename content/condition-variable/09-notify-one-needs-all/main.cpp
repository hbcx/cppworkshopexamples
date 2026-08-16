#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    // ANTI-PATTERN: using notify_one when a single state change must release SEVERAL
    // waiters. notify_one wakes exactly one of them; the rest keep sleeping, and if
    // nothing else ever notifies, they hang forever.
    //
    //   go = true;
    //   cv.notify_one();          // wakes ONE of the 8 waiters; 7 stay asleep -> hang
    //
    // We do NOT run that: seven stuck threads would never join and the build would
    // hang. The fix is notify_all when one event should free every waiter.

    std::mutex m;
    std::condition_variable cv;
    bool go = false;
    std::atomic<int> released{0};

    std::vector<std::thread> workers;
    for (int i = 0; i < 8; ++i)
        workers.emplace_back([&] {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&] { return go; });
            lock.unlock();
            released.fetch_add(1);
        });

    {
        std::lock_guard<std::mutex> lock(m);
        go = true;
    }
    cv.notify_all();               // FIX: wake ALL eight waiters at once

    for (auto& t : workers) t.join();
    std::cout << "released = " << released.load() << '\n';   // 8

    // Rule of thumb: notify_one fits a single-item handoff (one queued item wakes one
    // consumer); notify_all fits one change that satisfies many waiters (a gate, a
    // broadcast, a shutdown). A too-narrow notify_one is a classic cause of a thread
    // that mysteriously never wakes. When in doubt, notify_all.
    return 0;
}
