#include <cstddef>
#include <iostream>
#include <memory>

// unique_ptr<T[]> is the array specialization: it calls delete[] (not delete),
// gives you operator[], and deliberately has NO operator* or ->. It owns a raw
// dynamic array -- but a std::vector, which also tracks the size and can grow,
// is almost always the better choice.
int main() {
    const std::size_t n = 5;
    std::unique_ptr<int[]> buf(new int[n]);   // note the [] in the type
    for (std::size_t i = 0; i < n; ++i) {
        buf[i] = static_cast<int>(i * i);     // operator[] indexes the array
    }

    std::cout << "buf:";
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << ' ' << buf[i];
    }
    std::cout << '\n';

    // get() yields the raw int*, ready to hand to a C API expecting pointer + length.
    std::cout << "first element via get(): " << buf.get()[0] << '\n';
    return 0;                                 // delete[] runs automatically here
}
