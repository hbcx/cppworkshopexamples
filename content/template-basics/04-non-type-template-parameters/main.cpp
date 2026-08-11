#include <iostream>

// A non-type template parameter is a VALUE baked into the type. The array size N
// is part of Buffer<T, N>'s type, so different sizes are different types.
template <class T, int N>
struct Buffer {
    T data[N];
    constexpr int size() const { return N; }
};

// C++17 template <auto V> deduces the value's TYPE from the constant.
template <auto V>
struct Constant {
    static constexpr auto value = V;
};

int main() {
    Buffer<int, 4> b{};
    std::cout << "Buffer<int,4>::size() = " << b.size() << "\n";   // 4
    b.data[0] = 10;
    std::cout << "b.data[0] = " << b.data[0] << "\n";

    std::cout << "Constant<42>::value  = " << Constant<42>::value << "\n";    // 42 (int)
    std::cout << "Constant<'A'>::value = " << Constant<'A'>::value << "\n";   // A (char)
    return 0;
}
