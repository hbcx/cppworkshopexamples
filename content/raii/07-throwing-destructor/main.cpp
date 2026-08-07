// Anti-pattern: throwing an exception from a destructor.
//
// Destructors run during stack unwinding -- exactly when another exception may
// already be in flight. If a destructor throws then, C++ cannot choose between
// two exceptions and calls std::terminate: the program dies. Since C++11
// destructors are noexcept by default, so a throw that escapes one calls
// std::terminate even without a second exception in flight:
//
//     ~Saver() { if (dirty_) throw std::runtime_error("flush failed"); }  // TERMINATES
//
// We do NOT run that -- it would abort the process. The rule: a destructor must
// not let an exception escape. If releasing can fail in a way the caller needs
// to know about, expose an explicit commit()/close() that CAN report the error,
// and let the destructor be a silent last-resort backstop.

#include <iostream>
#include <stdexcept>

class Saver {
public:
    Saver() { std::cout << "open\n"; }

    // The fallible operation, as an explicit call. It CAN throw, and because it
    // is not a destructor the caller can catch it.
    void commit() {
        if (done_) return;
        std::cout << "commit: flushing\n";
        // If the flush failed we would throw here, and the CALLER handles it.
        done_ = true;
    }

    ~Saver() noexcept {
        if (!done_) {
            // Backstop: commit() was never called (perhaps an exception skipped
            // it). Clean up, but do NOT throw -- swallow any error.
            std::cout << "~Saver: discarding uncommitted work (no throw)\n";
        } else {
            std::cout << "~Saver: already committed, nothing to do\n";
        }
    }
private:
    bool done_ = false;   // C++11 in-class initialiser
};

int main() {
    std::cout << "-- normal: commit, then destroy --\n";
    {
        Saver s;
        s.commit();          // a failure here is catchable, unlike in a destructor
    }

    std::cout << "\n-- early exit: commit skipped, destructor backstops --\n";
    try {
        Saver s;
        throw std::runtime_error("boom");   // skips s.commit(); ~Saver must not throw
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << "\n";
    }
}
