// Anti-pattern: treating a plain char buffer as storage for a wider type.
//
// A char array is 1-byte aligned. A double needs 8. Accessing a double through
// a pointer into an unaligned buffer is a misaligned access -- undefined, and a
// hardware fault on CPUs that require alignment. The fix: alignas on the buffer.

#include <cstdint>
#include <iostream>
#include <new>   // placement new

int main() {
    // BAD (misaligned -- described, not run):
    //   char buf[sizeof(double)];                 // alignment 1
    //   double* d = reinterpret_cast<double*>(buf);
    //   *d = 3.14;   // 8-byte store to a maybe-odd address: undefined

    // GOOD: align the storage to the type, then construct in it.
    alignas(double) unsigned char buf[sizeof(double)];
    double* d = new (buf) double(3.14);   // aligned, lifetime begun properly

    bool ok = reinterpret_cast<std::uintptr_t>(d) % alignof(double) == 0;
    std::cout << "buffer aligned for double? " << (ok ? "yes" : "no") << '\n';
    std::cout << "value read back: " << *d << '\n';
    // double is trivial, so no destructor call is needed here.
}
