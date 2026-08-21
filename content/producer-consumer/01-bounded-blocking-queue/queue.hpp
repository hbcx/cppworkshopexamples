#pragma once
// A reusable, header-only bounded blocking queue for the producer-consumer
// pattern. Copy this file into your own code and store whatever type T you like.
//
// Design in one line: a std::mutex guards the state, one condition_variable
// wakes producers when the queue stops being full, another wakes consumers when
// it stops being empty. Every read and write of the shared state happens under
// the mutex; the condition variables carry no data, they only wake threads.
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

template <class T>
class BoundedBlockingQueue {
public:
    explicit BoundedBlockingQueue(std::size_t capacity) : capacity_(capacity) {}

    // Enqueue, blocking while the queue is full (this is the backpressure: a
    // fast producer is made to wait for a slow consumer). Returns false if the
    // queue was closed instead of accepting the item.
    bool push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this] { return queue_.size() < capacity_ || closed_; });
        if (closed_) return false;
        queue_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    // Dequeue, blocking while the queue is empty. Returns std::nullopt only when
    // the queue is closed AND fully drained, which is the signal for a consumer
    // loop to stop: while (auto item = q.pop()) { use(*item); }
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !queue_.empty() || closed_; });
        if (queue_.empty()) return std::nullopt; // closed and drained
        T value = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return value;
    }

    // Stop the queue. Producers still blocked in push wake and get false; the
    // items already queued stay, so consumers drain them before pop returns
    // nullopt. notify_all because every waiter must re-check the closed flag.
    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        not_full_.notify_all();
        not_empty_.notify_all();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    std::queue<T> queue_;
    std::size_t capacity_;
    bool closed_ = false;
};
