// alignas raises alignment above the natural requirement. Here a 4-float struct
// that would only need 4-byte alignment is pushed to 16, so every object of it
// starts on a 16-byte boundary -- what a SIMD load expects.

#include <cstdint>
#include <iostream>

struct Plain {
    float v[4];   // natural alignment: 4 (a float)
};

struct alignas(16) Vec4 {
    float v[4];   // same data, but alignas raises the boundary to 16
};

int main() {
    std::cout << "alignof(Plain) = " << alignof(Plain) << '\n';
    std::cout << "alignof(Vec4)  = " << alignof(Vec4)  << '\n';

    Vec4 vec{};
    bool aligned = reinterpret_cast<std::uintptr_t>(&vec) % 16 == 0;
    std::cout << "Vec4 object on a 16-byte boundary? "
              << (aligned ? "yes" : "no") << '\n';
}
