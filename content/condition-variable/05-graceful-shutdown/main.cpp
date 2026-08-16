#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

// A long-lived worker thread that sleeps on a condition variable while idle and is
// woken either by new work or by a stop request. This is how you shut a worker down
// cleanly: the stop flag plus a notify wakes it out of its wait so it can exit,
// instead of it spinning or blocking forever.
class Worker {
public:
    Worker() : t_([this] { run(); }) {}

    void submit(int job) {
        {
            std::lock_guard<std::mutex> lock(m_);
            jobs_.push(job);
        }
        cv_.notify_one();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(m_);
            stopping_ = true;
        }
        cv_.notify_one();            // wake the worker even if idle, so it can exit
        t_.join();
    }

    long total() const { return total_; }

private:
    void run() {
        for (;;) {
            std::unique_lock<std::mutex> lock(m_);
            cv_.wait(lock, [this] { return !jobs_.empty() || stopping_; });
            while (!jobs_.empty()) { total_ += jobs_.front(); jobs_.pop(); }
            if (stopping_) return;   // drained and asked to stop
        }
    }

    std::mutex m_;
    std::condition_variable cv_;
    std::queue<int> jobs_;
    bool stopping_ = false;
    long total_ = 0;
    std::thread t_;   // declared LAST: the thread starts only after the members above
                      // are fully constructed, so run() never reads an uninitialised one
};

int main() {
    Worker w;
    for (int i = 1; i <= 50; ++i) w.submit(i);
    w.stop();                        // request stop, wake the worker, join it
    std::cout << "worker total = " << w.total() << '\n';   // 1..50 = 1275
    return 0;
}
