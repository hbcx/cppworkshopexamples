#pragma once
// A fixed-size thread pool -- the producer-consumer pattern applied. The "items"
// are tasks (std::function<void()>); callers are the producers, the worker
// threads are the consumers. The bounded queue gives submit() its backpressure
// for free: if the workers fall behind and the task queue fills, submit blocks.
#include "queue.hpp"

#include <cstddef>
#include <functional>
#include <thread>
#include <utility>
#include <vector>

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(std::size_t workers, std::size_t queueCapacity)
        : tasks_(queueCapacity) {
        for (std::size_t i = 0; i < workers; ++i)
            workers_.emplace_back([this] {
                // Each worker pulls and runs tasks until the queue is closed and
                // drained (pop returns nullopt).
                while (std::optional<Task> task = tasks_.pop())
                    (*task)();
            });
    }

    // Hand a task to the pool. Blocks while the task queue is full (backpressure
    // on the submitter). Returns false if the pool has been shut down.
    bool submit(Task task) { return tasks_.push(std::move(task)); }

    // Stop accepting work, let the workers finish everything already queued,
    // then join them. Safe to call more than once.
    void shutdown() {
        tasks_.close();
        for (std::thread& t : workers_)
            if (t.joinable()) t.join();
        workers_.clear();
    }

    ~ThreadPool() { shutdown(); }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    BoundedBlockingQueue<Task> tasks_;
    std::vector<std::thread> workers_;
};
