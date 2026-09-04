#include <iostream>
#include <vector>
#include <iterator>   // std::data, std::size (C++17)
#include <cstddef>    // std::size_t

// A C API expecting a pointer plus a length.
static void send(const int* data, std::size_t n) {
    std::cout << "sending " << n << " element(s):";
    for (std::size_t i = 0; i < n; ++i) std::cout << " " << data[i];
    std::cout << "\n";
}

int main() {
    std::vector<int> empty;

    // ANTI-PATTERN: assuming std::data(v) points at a readable element. For an
    // empty vector std::data may be nullptr, and even otherwise the address is
    // one past the end, so reading it is undefined behaviour:
    //
    //   int first = *std::data(empty);   // UB: there is no element to read
    //
    // Comparing the pointer is defined; dereferencing it is not.
    std::cout << std::boolalpha
              << "data(empty) == nullptr? " << (std::data(empty) == nullptr) << "\n";

    // FIX: gate every use of data() on size(). send() reads nothing when n == 0,
    // so the null / one-past pointer is never dereferenced. This is the safe
    // replacement for the old &v[0], which is itself UB on an empty vector.
    send(std::data(empty), std::size(empty));

    std::vector<int> v = {11, 22, 33};
    send(std::data(v), std::size(v));
    return 0;
}
