#include <iostream>
#include <memory>

int main() {
    // C++17: shared_ptr<T[]> knows the buffer came from new[] and will release it
    // with delete[]. It gives operator[] for indexing and, deliberately, no
    // operator* or -> because it owns an array, not a single object.
    // (make_shared<T[]> only arrives in C++20, so build from new[] here.)
    std::shared_ptr<int[]> buf(new int[4]);

    for (int i = 0; i < 4; ++i)
        buf[i] = (i + 1) * 10;          // operator[], the array-form accessor

    std::cout << "buf[2] = " << buf[2] << '\n';

    // Shared ownership works exactly as for the single-object form: this is a
    // second owner of the SAME array, sharing one reference count.
    std::shared_ptr<int[]> other = buf;
    other[0] = 99;
    std::cout << "buf[0] written through the other owner = " << buf[0] << '\n';
    std::cout << "use_count = " << buf.use_count() << '\n';   // 2

    return 0;   // last owner drops -> the array is freed once, with delete[]
}
