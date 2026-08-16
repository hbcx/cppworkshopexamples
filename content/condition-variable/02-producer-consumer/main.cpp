#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main() {
    // The job a condition variable is built for: a producer/consumer queue. The
    // consumer sleeps while the queue is empty and wakes when there is work, instead
    // of spinning on the queue. One mutex guards the queue; the cv signals "state
    // changed"; a 'done' flag tells the consumer no more items are coming.
    std::mutex m;
    std::condition_variable cv;
    std::queue<int> q;
    bool done = false;
    long sum = 0;

    std::thread consumer([&] {
        for (;;) {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [&] { return !q.empty() || done; });   // work, or the end
            while (!q.empty()) {                 // drain everything available
                sum += q.front();
                q.pop();
            }
            if (done) break;                     // empty AND producer finished -> exit
        }
    });

    // Producer: push items under the lock and notify after each.
    for (int i = 1; i <= 100; ++i) {
        {
            std::lock_guard<std::mutex> lock(m);
            q.push(i);
        }
        cv.notify_one();
    }
    // Signal end-of-stream, then wake the consumer one last time so it can exit.
    {
        std::lock_guard<std::mutex> lock(m);
        done = true;
    }
    cv.notify_one();

    consumer.join();
    std::cout << "consumed sum = " << sum << '\n';   // 1..100 = 5050
    return 0;
}
