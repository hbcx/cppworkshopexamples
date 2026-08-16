#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

// A task on another thread cannot just throw an exception at you -- the throw
// happens on a different stack. Futures solve this: if the task throws, the
// exception is CAPTURED and stored in the future, and re-thrown when you call
// get(). So error handling looks the same as a normal call: wrap get() in
// try/catch. This works for std::async automatically, and for std::promise via
// set_exception.
long risky(int x) {
    if (x < 0) throw std::runtime_error("negative input not allowed");
    return x * 10L;
}

int main() {
    // --- async: a throw inside the task surfaces at get() ---
    std::future<long> f = std::async(std::launch::async, risky, -1);
    try {
        long r = f.get();                 // re-throws the exception here
        std::cout << "got " << r << '\n';
    } catch (const std::exception& e) {
        std::cout << "async task threw: " << e.what() << '\n';
    }

    // --- promise: forward an exception by hand with set_exception ---
    std::promise<long> p;
    std::future<long> pf = p.get_future();
    std::thread producer([&p] {
        try {
            p.set_value(risky(-5));       // risky throws...
        } catch (...) {
            // ...so send the in-flight exception down the channel instead.
            p.set_exception(std::current_exception());
        }
    });
    try {
        pf.get();
    } catch (const std::exception& e) {
        std::cout << "promise forwarded: " << e.what() << '\n';
    }
    producer.join();
    return 0;
}
