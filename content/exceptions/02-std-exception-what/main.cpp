// A few different standard exception types, thrown by index. They live in
// different headers and mean different things, but they all derive from
// std::exception, so a single catch (const std::exception&) handles them all
// and what() returns each one's message.
#include <iostream>
#include <stdexcept>

void fail(int which) {
    switch (which) {
        case 0: throw std::runtime_error("disk write failed");
        case 1: throw std::out_of_range("row index 10 past end 3");
        case 2: throw std::invalid_argument("expected a number");
        default: return;
    }
}

int main() {
    for (int i = 0; i < 3; ++i) {
        try {
            fail(i);
        }
        // One handler for the whole std::exception family. what() returns the
        // message passed to the constructor.
        catch (const std::exception& e) {
            std::cout << "caught: " << e.what() << '\n';
        }
    }
    return 0;
}
