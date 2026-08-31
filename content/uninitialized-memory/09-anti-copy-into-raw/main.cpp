// Anti-pattern: std::copy (or plain assignment) into raw storage.
//
// std::copy does  dest[i] = src[i]  -- an ASSIGNMENT. For a std::string that
// reads the destination's existing pointer and length to release the old
// buffer. In raw storage there is no old string, just garbage bytes, so the
// assignment frees a garbage pointer. It compiles cleanly and corrupts the heap.

#include <algorithm>
#include <iostream>
#include <memory>   // std::uninitialized_copy
#include <string>
#include <vector>

int main() {
    using String = std::string;
    std::vector<String> src = {"north", "south", "east"};

    String* buf = static_cast<String*>(
        ::operator new(sizeof(String) * src.size()));

    // BAD (undefined -- described, not run):
    //   std::copy(src.begin(), src.end(), buf);
    // buf[i] = src[i] assigns onto a std::string that was never constructed;
    // operator= reads a junk pointer/length from buf[i] and frees it.

    // GOOD: uninitialized_copy runs the COPY CONSTRUCTOR in each raw slot,
    // so no assignment ever touches a non-existent object.
    String* end = std::uninitialized_copy(src.begin(), src.end(), buf);

    std::cout << "uninitialized_copy built " << (end - buf) << " strings:\n";
    for (String* p = buf; p != end; ++p) std::cout << "  " << *p << '\n';

    for (String* p = buf; p != end; ++p) p->~String();
    ::operator delete(buf);
}
