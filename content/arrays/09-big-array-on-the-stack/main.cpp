#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

namespace {

// Typical default thread stacks. Neither is a standard guarantee -- they are
// what you get if nobody configured anything.
constexpr std::size_t kWindowsStack = 1024u * 1024u;        // 1 MB
constexpr std::size_t kLinuxStack = 8u * 1024u * 1024u;     // 8 MB

void report(const char* label, std::size_t bytes) {
    std::cout << "  " << label << " = " << bytes / 1024 << " KB"
              << (bytes >= kWindowsStack ? "   <- over a 1 MB stack" : "")
              << (bytes >= kLinuxStack ? " and over an 8 MB one" : "") << '\n';
}

// The array is passed BY REFERENCE: no copy, no second frame the size of the
// data. Taking it by value would copy every element at every call.
long long sum(const std::array<int, 1024>& values) {
    long long total = 0;
    for (int v : values) {
        total += v;
    }
    return total;
}

}  // namespace

int main() {
    // sizeof tells the whole story: the elements ARE the object, so the type's
    // size grows with N and a local variable of it is that much stack frame.
    std::cout << "sizeof(std::array<int, N>):\n";
    report("N = 1 000     ", sizeof(std::array<int, 1000>));
    report("N = 100 000   ", sizeof(std::array<int, 100000>));
    report("N = 1 000 000 ", sizeof(std::array<int, 1000000>));

    std::cout << "\nints that fit in a default stack (whole frame, nothing else):\n";
    std::cout << "  Windows, 1 MB: " << kWindowsStack / sizeof(int) << '\n';
    std::cout << "  Linux,   8 MB: " << kLinuxStack / sizeof(int) << '\n';

    // Anti-pattern (shown, NOT run): this is not a bad_alloc you can catch, it
    // is the guard page. The process dies, often blaming the wrong line:
    //   std::array<int, 1000000> buffer{};   // ~4 MB of stack frame
    //
    // Nor is it only about declaring one. Passing an array BY VALUE copies it,
    // so the size that overflowed the frame is paid again at every call:
    //   long long sum(std::array<int, 1000000> values);   // a 4 MB copy

    // Fix 1: the same elements on the heap, for one allocation.
    std::vector<int> onHeap(1000000, 1);
    std::cout << "\nvector<int>(1000000): " << onHeap.size()
              << " ints on the heap, the local object is only "
              << sizeof(onHeap) << " bytes\n";

    // Fix 2: keep the array, put it on the heap yourself. Useful when the fixed
    // size is part of the design and you want it in the type.
    auto boxed = std::make_unique<std::array<int, 1000000>>();
    boxed->fill(2);
    std::cout << "unique_ptr<array<int,1000000>>: element 999999 = "
              << (*boxed)[999999] << ", local object is " << sizeof(boxed) << " bytes\n";

    // Fix 3: a small array is fine on the stack -- pass it by reference so the
    // size is not copied at the boundary.
    std::array<int, 1024> small{};
    small.fill(3);
    std::cout << "array<int,1024> (" << sizeof(small) / 1024
              << " KB) by reference: sum = " << sum(small) << '\n';
    return 0;
}
