#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>

int main() {
    std::cout << std::boolalpha;

    std::mutex m;
    std::condition_variable cv;
    bool ready = false;

    // wait_for(lock, duration, predicate) is wait with a deadline. It returns the
    // value of the PREDICATE: true if the condition held before the timeout, false
    // if the wait timed out. That lets you take a timeout path instead of blocking
    // forever -- retry, log, or give up.

    // Case 1: the condition is ALREADY true, so wait_for returns true immediately,
    // without waiting out the timeout.
    {
        std::lock_guard<std::mutex> lock(m);
        ready = true;
    }
    {
        std::unique_lock<std::mutex> lock(m);
        bool got = cv.wait_for(lock, std::chrono::seconds(5), [&] { return ready; });
        std::cout << "case 1 (already ready): " << got << '\n';   // true, returns at once
    }

    // Case 2: nobody ever sets the condition, so wait_for waits out the (short)
    // timeout and returns false -- the predicate never became true.
    ready = false;
    {
        std::unique_lock<std::mutex> lock(m);
        bool got = cv.wait_for(lock, std::chrono::milliseconds(50), [&] { return ready; });
        std::cout << "case 2 (times out):    " << got << '\n';    // false
    }

    // wait_until does the same against an ABSOLUTE time point, which is the right
    // choice when you must not drift across several waits (a fixed overall deadline).
    return 0;
}
