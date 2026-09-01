#include <mdspan>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::mdspan grid(v.data(), 2, 3);
    std::cout << "before growth: grid[0,0] = " << grid[0, 0] << "\n";

    // TRAP: 'grid' holds v's OLD data pointer. Growing the vector past its
    // capacity reallocates: the elements move to a new buffer and the old one
    // is freed. 'grid' now points at freed memory, so reading through it is
    // undefined behaviour (a dangling view). We do NOT run that line:
    for (int i = 0; i < 100; ++i) v.push_back(i);
    // std::cout << grid[0, 0];   // <-- undefined behaviour: dangling pointer

    // FIX: rebuild the view from the current pointer, after the storage settled.
    std::mdspan fixed(v.data(), 2, 3);
    std::cout << "after growth:  fixed[0,0] = " << fixed[0, 0] << "\n";
    std::cout << "(rebuild the mdspan whenever the buffer may have moved)\n";
}
