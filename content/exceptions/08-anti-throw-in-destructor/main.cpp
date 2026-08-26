// Anti-pattern: throwing from a destructor. If the destructor runs while ANOTHER
// exception is already propagating (stack unwinding), there are now two
// exceptions in flight, which C++ resolves by calling std::terminate -- the
// program aborts and no catch runs. Destructors are implicitly noexcept since
// C++11, so any escaping exception terminates. Never let one leave a destructor.
#include <iostream>
#include <stdexcept>

// WRONG (described, not run): a destructor that lets an exception escape.
//
//   struct Bad {
//       ~Bad() { throw std::runtime_error("cleanup failed"); }  // escapes
//   };
//   void f() {
//       Bad b;
//       throw std::runtime_error("first");   // during unwinding, ~Bad throws
//   }                                        // -> two in flight -> std::terminate
//
// Running that would abort the program, so we do not. Instead, the fix:

// RIGHT: the destructor does its cleanup but HANDLES any failure internally --
// it never lets an exception out. Swallow or log; do not rethrow.
struct Good {
    ~Good() {
        try {
            throw std::runtime_error("cleanup failed");   // pretend cleanup fails
        }
        catch (const std::exception& e) {
            std::cout << "destructor handled: " << e.what() << '\n';
        }
    }
};

int main() {
    try {
        Good g;
        throw std::runtime_error("first");     // g is destroyed as this unwinds
    }
    catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << '\n';
    }
    return 0;
}
