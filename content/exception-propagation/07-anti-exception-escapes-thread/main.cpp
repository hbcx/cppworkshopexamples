// Anti-pattern: letting an exception escape a std::thread's function. If the
// thread's top-level function throws and does not catch it, the exception has
// nowhere to go -- std::terminate is called and the whole program aborts, not
// just the thread. A worker must catch its own exceptions and hand them back
// (via exception_ptr) or handle them in place.
#include <iostream>
#include <exception>
#include <stdexcept>
#include <thread>
#include <functional>

// WRONG (described, not run -- it would abort the program):
//
//   void bad_worker() {
//       throw std::runtime_error("boom");   // escapes the thread function
//   }
//   std::thread t(bad_worker);
//   t.join();                               // -> std::terminate, whole app aborts
//
// Wrapping the join in try/catch does NOT help: the program has already
// terminated by the time join would rethrow.

// RIGHT: the worker contains its exceptions and reports back through a slot.
void good_worker(std::exception_ptr& slot) {
    try {
        throw std::runtime_error("boom");
    }
    catch (...) {
        slot = std::current_exception();       // safe: handed back as a value
    }
}

int main() {
    std::exception_ptr slot;
    std::thread t(good_worker, std::ref(slot));
    t.join();

    if (slot) {
        try {
            std::rethrow_exception(slot);
        }
        catch (const std::exception& e) {
            std::cout << "main handled it safely: " << e.what() << '\n';
        }
    }
    return 0;
}
