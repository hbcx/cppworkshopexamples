// Anti-pattern: a custom what() that returns a pointer into a temporary. what()
// returns const char* and the pointer MUST stay valid after it returns. Building
// the string inside what() and returning .c_str() dangles -- the temporary is
// destroyed at the semicolon, and the caller reads freed memory.
#include <iostream>
#include <stdexcept>
#include <string>

// WRONG (described, not run -- it is undefined behaviour):
//
//   struct BadError : std::exception {
//       std::string detail;
//       const char* what() const noexcept override {
//           return ("load failed: " + detail).c_str();  // temporary dies here
//       }                                               // -> dangling pointer
//   };
//
// Both safe options keep the message alive:

// RIGHT (a): let std::runtime_error store the message. It copies the string, so
// what() returns a pointer into the stored copy -- always valid, and simplest.
struct LoadErrorA : std::runtime_error {
    explicit LoadErrorA(const std::string& detail)
        : std::runtime_error("load failed: " + detail) {}
};

// RIGHT (b): if you must override what(), build the message ONCE in the
// constructor into a member, and return a pointer into that member.
struct LoadErrorB : std::exception {
    std::string message;
    explicit LoadErrorB(const std::string& detail)
        : message("load failed: " + detail) {}
    const char* what() const noexcept override { return message.c_str(); }
};

int main() {
    try { throw LoadErrorA("disk offline"); }
    catch (const std::exception& e) { std::cout << "A: " << e.what() << '\n'; }

    try { throw LoadErrorB("disk offline"); }
    catch (const std::exception& e) { std::cout << "B: " << e.what() << '\n'; }
    return 0;
}
