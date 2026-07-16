#include <atomic>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// A queue that locks every method LOOKS thread-safe and is not:
//
//   class LockedQueue {
//       std::queue<int> q; std::mutex m;
//   public:
//       bool empty()  { std::lock_guard<std::mutex> g(m); return q.empty(); }
//       int  front()  { std::lock_guard<std::mutex> g(m); return q.front(); }
//       void pop()    { std::lock_guard<std::mutex> g(m); q.pop(); }
//   };
//
// because the CALLER still has to compose three separate critical sections:
//
//   if (!shared.empty()) {   // true -- one element left
//                            // <-- another consumer pops it right here
//       int v = shared.front();   // UB: reads an element that no longer exists
//       shared.pop();             // UB: pops an empty queue
//   }
//
// Every lock is held correctly; the race is in the gaps BETWEEN them. Nothing
// below runs that version -- a data race is undefined behaviour, so we only run
// the fix.

// The fix: the whole check-take-remove is one critical section, and it is the
// only way the queue can be consumed, so no caller can reintroduce the gap.
class TaskQueue {
public:
    void push(int value) {
        std::lock_guard<std::mutex> guard(mutex_);
        queue_.push(value);
    }

    // Returns false when empty -- there is no separate empty() to race against.
    bool try_pop(int& out) {
        std::lock_guard<std::mutex> guard(mutex_);
        if (queue_.empty()) {
            return false;
        }
        out = queue_.front();
        queue_.pop();
        return true;
    }

private:
    std::queue<int> queue_;
    std::mutex mutex_;
};

int main() {
    const int itemCount = 500;
    TaskQueue tasks;
    std::atomic<bool> producing(true);
    std::atomic<int> consumed(0);
    std::atomic<long long> total(0);

    std::thread producer([&] {
        for (int i = 1; i <= itemCount; ++i) {
            tasks.push(i);
        }
        producing = false;
    });

    std::vector<std::thread> consumers;
    for (int c = 0; c < 3; ++c) {
        consumers.emplace_back([&] {
            for (;;) {
                // Read the flag BEFORE trying to pop. If the producer was already
                // finished at that point and the pop still finds nothing, the
                // queue is empty for good -- reading it the other way round could
                // miss items pushed between the failed pop and the flag check.
                const bool producerDone = !producing;
                int value = 0;
                if (tasks.try_pop(value)) {
                    total += value;
                    ++consumed;
                    continue;
                }
                if (producerDone) {
                    break;
                }
                std::this_thread::yield();
            }
        });
    }

    producer.join();
    for (auto& t : consumers) {
        t.join();
    }

    // The interleaving differs on every run; the totals do not. Every item is
    // taken exactly once, by exactly one consumer.
    std::cout << "items produced: " << itemCount << '\n';
    std::cout << "items consumed: " << consumed.load() << '\n';
    std::cout << "sum received:   " << total.load() << " (expected "
              << static_cast<long long>(itemCount) * (itemCount + 1) / 2 << ")\n";
    return 0;
}
