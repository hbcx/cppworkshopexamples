#include <functional>
#include <iostream>
#include <string>

struct Connection {
    std::string host;
    void send(const std::string& channel, const std::string& msg) const {
        std::cout << host << "/" << channel << ": " << msg << "\n";
    }
};

int add(int a, int b, int c) { return a + b + c; }

int main() {
    // std::bind_front (C++20) fixes the FIRST arguments and leaves the rest to
    // the call -- no placeholders, no _1/_2 ceremony. The remaining call
    // arguments are appended in order, so it cannot reorder or drop them.
    auto add5 = std::bind_front(add, 5);
    std::cout << "add5(10, 20) = " << add5(10, 20) << "\n";   // 35

    // The everyday case: pin an object and a member function to get a ready
    // callback -- cleaner than std::bind(&T::m, &obj, _1) with placeholders.
    Connection conn{"irc.example"};
    auto toGeneral = std::bind_front(&Connection::send, &conn, "general");
    toGeneral("hello");
    toGeneral("second line");

    // std::bind_back (C++23) is the mirror image -- it fixes the LAST arguments.
    // Shown as a comment because this example is compiled as C++20:
    //   auto f = std::bind_back(add, 100, 0);  f(1) calls add(1, 100, 0)
    return 0;
}
