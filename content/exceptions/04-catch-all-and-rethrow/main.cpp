// catch(...) catches anything, including exceptions you did not anticipate and
// types that are not std::exception at all. A bare `throw;` inside a handler
// RETHROWS the current exception unchanged -- useful to log at a boundary and
// let it keep propagating, or to translate it into a different type.
#include <iostream>
#include <stdexcept>

void library_call(int which) {
    if (which == 0) throw std::runtime_error("connection reset");
    if (which == 1) throw 42;               // a non-std::exception throw
}

// A boundary that logs every failure, then lets std::exception cases keep going
// and translates unknown throws into a std::runtime_error the caller expects.
void guarded(int which) {
    try {
        library_call(which);
    }
    catch (const std::exception& e) {
        std::cout << "log: standard error: " << e.what() << '\n';
        throw;                              // rethrow the SAME exception
    }
    catch (...) {
        std::cout << "log: non-standard throw\n";
        throw std::runtime_error("wrapped unknown error");   // translate
    }
}

int main() {
    for (int i = 0; i < 2; ++i) {
        try {
            guarded(i);
        }
        catch (const std::exception& e) {
            std::cout << "handled: " << e.what() << '\n';
        }
    }
    return 0;
}
