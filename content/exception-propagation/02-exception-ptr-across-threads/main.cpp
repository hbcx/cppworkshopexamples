// An exception cannot cross a thread boundary by itself -- it would escape the
// thread function and call std::terminate. std::exception_ptr is how you move it:
// the worker catches its own exception, stores it in an exception_ptr the main
// thread can see, and after join() the main thread rethrows and handles it.
#include <iostream>
#include <exception>
#include <stdexcept>
#include <thread>
#include <functional>

void worker(std::exception_ptr& slot, int input) {
    try {
        if (input < 0) throw std::runtime_error("worker got a negative input");
        std::cout << "worker ok: " << input << '\n';
    }
    catch (...) {
        slot = std::current_exception();       // hand the failure back as a value
    }
}

int main() {
    std::exception_ptr from_worker;

    std::thread t(worker, std::ref(from_worker), -5);
    t.join();                                   // synchronizes: the slot is safe to read

    if (from_worker) {
        try {
            std::rethrow_exception(from_worker);
        }
        catch (const std::exception& e) {
            std::cout << "main handled worker error: " << e.what() << '\n';
        }
    }
    return 0;
}
