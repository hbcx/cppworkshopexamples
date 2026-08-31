// polymorphic_allocator holds the resource as a run-time pointer, so two
// pmr::vector<int> with different memory strategies are the SAME type. A
// function can take one by reference and let the caller pick the resource --
// no templates, no recompiling per strategy.

#include <array>
#include <cstddef>
#include <iostream>
#include <memory_resource>
#include <type_traits>
#include <vector>

// Takes any pmr::vector<int> -- the resource is the caller's choice.
static void fill(std::pmr::vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) v.push_back(i);
}

int main() {
    std::array<std::byte, 512> buffer;
    std::pmr::monotonic_buffer_resource stackRes{buffer.data(), buffer.size()};

    std::pmr::vector<int> onStack{&stackRes};       // uses the stack buffer
    std::pmr::vector<int> onHeap;                   // uses the default heap resource
    onStack.reserve(8);
    onHeap.reserve(8);

    fill(onStack, 8);
    fill(onHeap, 8);

    std::cout << std::boolalpha;
    std::cout << "same type: "
              << std::is_same<decltype(onStack), decltype(onHeap)>::value << '\n';

    const std::byte* start = buffer.data();
    const std::byte* end = start + buffer.size();
    auto inBuffer = [&](const void* p) { return p >= start && p < end; };
    std::cout << "onStack allocates in the buffer: " << inBuffer(onStack.data()) << '\n';
    std::cout << "onHeap  allocates in the buffer: " << inBuffer(onHeap.data()) << '\n';
}
