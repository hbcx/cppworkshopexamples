// Anti-pattern: throwing std::exception itself. Its constructor takes no
// message, so what() returns a fixed, generic string with no detail about what
// went wrong -- a handler learns nothing and cannot log anything useful. Throw a
// specific type constructed with a real message instead.
#include <iostream>
#include <stdexcept>
#include <exception>

int main() {
    // WRONG: no message can be attached; what() is a generic placeholder.
    try {
        throw std::exception();
    }
    catch (const std::exception& e) {
        std::cout << "bare exception what(): " << e.what() << '\n';
    }

    // RIGHT: a specific type with a message the handler and logs can use.
    try {
        throw std::runtime_error("config file /etc/app.conf is missing");
    }
    catch (const std::exception& e) {
        std::cout << "specific what():       " << e.what() << '\n';
    }
    return 0;
}
