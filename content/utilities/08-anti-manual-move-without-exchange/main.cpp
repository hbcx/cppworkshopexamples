#include <iostream>
#include <utility>

struct Buffer {
    int* data = nullptr;

    Buffer() : data(new int[4]{}) {}
    ~Buffer() { delete[] data; }

    // FIX: std::exchange steals other.data AND resets it to nullptr in one
    // expression, so the moved-from Buffer's destructor deletes nullptr (a no-op)
    // and the resource has exactly one owner.
    Buffer(Buffer&& other) noexcept : data(std::exchange(other.data, nullptr)) {}

    // ANTI-PATTERN (described, NOT compiled -- it double-frees):
    //
    //   Buffer(Buffer&& other) noexcept : data(other.data) {}   // forgot to reset
    //
    // Now BOTH objects hold the same pointer. When both destructors run, the same
    // buffer is delete[]-ed twice -> undefined behaviour. Copying the pointer
    // without nulling the source is the classic move bug std::exchange prevents.

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
};

int main() {
    Buffer a;
    Buffer b = std::move(a);   // steals a's buffer, leaves a.data == nullptr

    std::cout << std::boolalpha;
    std::cout << "after move: source data is null? " << (a.data == nullptr) << "\n";  // true
    std::cout << "target has the buffer?           " << (b.data != nullptr) << "\n";  // true
    return 0;
}
