// A monotonic_buffer_resource on a stack array: the pmr::vector allocates by
// bumping a cursor through that array, never calling the heap. When the resource
// dies, the whole buffer is reclaimed at once.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <vector>

int main() {
    std::array<std::byte, 1024> buffer;
    std::pmr::monotonic_buffer_resource res{buffer.data(), buffer.size()};

    std::pmr::vector<int> v{&res};
    v.reserve(16);
    for (int i = 0; i < 16; ++i) v.push_back(i * 2);

    // Does the vector's storage live inside our stack buffer?
    const std::byte* start = buffer.data();
    const std::byte* end = start + buffer.size();
    const void* data = v.data();
    bool inside = data >= start && data < end;

    std::cout << "vector size: " << v.size() << '\n';
    std::cout << "storage inside the stack buffer (no heap)? "
              << std::boolalpha << inside << '\n';
}
