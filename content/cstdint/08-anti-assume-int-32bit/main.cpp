#include <cstdint>
#include <iostream>

// TRAP: writing a struct meant for a fixed binary layout with int / long. The
// standard only promises int >= 16 bits and long >= 32 bits; the ACTUAL width
// varies. long is 64 bits on 64-bit Linux and macOS (the LP64 model) but 32
// bits on 64-bit Windows (LLP64). A header defined with long changes size
// between platforms, so the two ends disagree on where every later field starts.
struct BadHeader  { long         id; long         length; };  // size moves with the platform!
struct GoodHeader { std::int64_t id; std::int64_t length; };  // always 16 bytes

int main() {
    // The built-in widths on THIS platform -- the numbers differ elsewhere:
    std::cout << "sizeof(int)  = " << sizeof(int)
              << " bytes\n";
    std::cout << "sizeof(long) = " << sizeof(long)
              << " bytes  <- 4 on Windows, 8 on Linux/macOS\n";

    std::cout << "\nsame header, two field types:\n";
    std::cout << "BadHeader  (long)    = " << sizeof(BadHeader)
              << " bytes  (moves with the platform)\n";
    std::cout << "GoodHeader (int64_t) = " << sizeof(GoodHeader)
              << " bytes  (fixed everywhere)\n";

    std::cout << "\nFIX: use int32_t / int64_t for any field whose size is part\n"
                 "of the contract -- files, sockets, shared memory, registers.\n";
}
