#pragma once
// The bounded blocking queue from example 01, extended with the operations you
// need when a thread must NOT block forever: non-blocking try_push / try_pop,
// and time-limited push_for / pop_for. Same design: a mutex, a not-full and a
// not-empty condition variable, all state touched only under the lock.
#include <chrono>
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

    bool push(T value) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this] { return queue_.size() < capacity_ || closed_; });
        if (closed_) return false;
        queue_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !queue_.empty() || closed_; });
        if (queue_.empty()) return std::nullopt;
        return take(lock);
    }

    // Enqueue only if there is room right now. Never blocks; returns false when
    // the queue is full or closed.
    bool try_push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (closed_ || queue_.size() >= capacity_) return false;
        queue_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    // Dequeue only if an item is available right now. Never blocks; returns
    // nullopt when the queue is empty (whether or not it is closed).
    std::optional<T> try_pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.empty()) return std::nullopt;
        return take(lock);
    }

    // Enqueue, waiting at most the given duration for room. Returns false on
    // timeout or if the queue was closed.
    template <class Rep, class Period>
    bool push_for(T value, const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!not_full_.wait_for(lock, timeout,
                                [this] { return queue_.size() < capacity_ || closed_; }))
            return false; // timed out
        if (closed_) return false;
        queue_.push(std::move(value));
        not_empty_.notify_one();
        return true;
    }

    // Dequeue, waiting at most the given duration for an item. Returns nullopt
    // on timeout, or when the queue is closed and drained.
    template <class Rep, class Period>
    std::optional<T> pop_for(const std::chrono::duration<Rep, Period>& timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!not_empty_.wait_for(lock, timeout,
                                 [this] { return !queue_.empty() || closed_; }))
            return std::nullopt; // timed out
        if (queue_.empty()) return std::nullopt; // closed and drained
        return take(lock);
    }

    void close() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            closed_ = true;
        }
        not_full_.notify_all();
        not_empty_.notify_all();
    }

private:
    // Pop the front item and wake one blocked producer. Caller holds the lock.
    T take(std::unique_lock<std::mutex>&) {
        T value = std::move(queue_.front());
        queue_.pop();
        not_full_.notify_one();
        return value;
    }

    mutable std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    std::queue<T> queue_;
    std::size_t capacity_;
    bool closed_ = false;
};
