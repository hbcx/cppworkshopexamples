// The _n variants: uninitialized_copy_n and uninitialized_fill_n take a COUNT
// instead of an end iterator, and each returns one past the last object it
// built. That return value lets you chain construction into one buffer and
// remember exactly how far the live region reaches.

#include <iostream>
#include <memory>   // std::uninitialized_copy_n, std::uninitialized_fill_n
#include <string>
#include <vector>

int main() {
    using String = std::string;
    std::vector<String> src = {"log-1", "log-2", "log-3", "log-4"};

    const std::size_t capacity = 6;
    String* buf = static_cast<String*>(::operator new(sizeof(String) * capacity));

    // Copy the first 3 source elements into the front of the buffer.
    // Returns the position right after the third constructed string.
    String* mid = std::uninitialized_copy_n(src.begin(), 3, buf);

    // Fill the next 2 slots with a placeholder, starting where copy_n stopped.
    // Returns the new end of the constructed region.
    String* end = std::uninitialized_fill_n(mid, 2, String("----"));

    std::cout << "copied 3, filled 2 -> live region holds "
              << (end - buf) << " strings:\n";
    for (String* p = buf; p != end; ++p) {
        std::cout << "  " << *p << '\n';
    }

    // Destroy exactly the region we constructed (buf .. end), not the whole
    // capacity -- the last slot was never built.
    for (String* p = buf; p != end; ++p) {
        p->~String();
    }
    ::operator delete(buf);
}
