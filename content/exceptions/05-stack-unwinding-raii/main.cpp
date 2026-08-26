// When an exception propagates, the stack UNWINDS: every local object between
// the throw and the catch is destroyed, in reverse order of construction. That
// is what makes RAII exception-safe -- a resource wrapped in a destructor is
// released even when control leaves through a throw, with no cleanup code on the
// error path.
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

// A stand-in for a resource (file handle, lock, connection). It reports its
// destruction so we can watch the unwinding happen.
struct Resource {
    std::string name;
    explicit Resource(std::string n) : name(std::move(n)) {
        std::cout << "acquire " << name << '\n';
    }
    ~Resource() {
        std::cout << "release " << name << '\n';
    }
};

void inner() {
    Resource b("B");
    throw std::runtime_error("failure in inner");    // stack starts unwinding here
}

void outer() {
    Resource a("A");
    inner();                                          // A is still alive here
}

int main() {
    try {
        outer();
    }
    catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << '\n';
    }
    return 0;
}
