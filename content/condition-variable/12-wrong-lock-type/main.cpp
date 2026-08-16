#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
    // ANTI-PATTERN: trying to use std::condition_variable with a lock other than
    // std::unique_lock<std::mutex>. condition_variable's wait ONLY accepts a
    // unique_lock<std::mutex> -- it will not compile with a unique_lock over a
    // recursive_mutex, a shared_lock, or any custom lockable.
    //
    //   std::recursive_mutex rm;
    //   std::unique_lock<std::recursive_mutex> lock(rm);
    //   std::condition_variable cv;
    //   cv.wait(lock, pred);      // does NOT compile: wrong lock type
    //
    // FIX: std::condition_variable_any waits with ANY lock type that meets the
    // BasicLockable requirement (lock()/unlock()). Use it when you must wait while
    // holding something other than a plain unique_lock<std::mutex>. It is slightly
    // more expensive, so prefer plain condition_variable when a unique_lock<mutex>
    // works.

    std::recursive_mutex rm;
    std::condition_variable_any cv;      // the "_any" version accepts other lock types
    bool ready = false;

    std::thread consumer([&] {
        std::unique_lock<std::recursive_mutex> lock(rm);
        cv.wait(lock, [&] { return ready; });    // works with condition_variable_any
        std::cout << "consumer proceeded (condition_variable_any)\n";
    });

    {
        std::lock_guard<std::recursive_mutex> lock(rm);
        ready = true;
    }
    cv.notify_one();

    consumer.join();

    // Takeaway: plain std::condition_variable is fixed to unique_lock<std::mutex>
    // (which makes it a little faster); std::condition_variable_any is the general
    // version for any other lock -- a recursive_mutex, a shared_mutex reader lock, or
    // a custom one. Reach for _any only when you actually need that generality.
    return 0;
}
