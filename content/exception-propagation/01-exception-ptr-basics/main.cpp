// std::exception_ptr turns "the exception currently being handled" into a VALUE
// you can copy, store and move. std::current_exception() (called inside a catch)
// captures it; std::rethrow_exception() throws it again later, even in another
// scope. This decouples WHERE an error is caught from WHERE it is handled.
#include <iostream>
#include <exception>
#include <stdexcept>

int main() {
    std::exception_ptr saved;          // empty: holds no exception yet

    // Catch here, but do not handle yet -- just capture the exception as a value.
    try {
        throw std::runtime_error("primary task failed");
    }
    catch (...) {
        saved = std::current_exception();
        std::cout << "captured, will handle later\n";
    }

    // ...later, somewhere else entirely, decide to handle it.
    if (saved) {
        try {
            std::rethrow_exception(saved);     // throw the stored exception again
        }
        catch (const std::exception& e) {
            std::cout << "handled: " << e.what() << '\n';
        }
    }
    return 0;
}
