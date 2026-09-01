#include <mdspan>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::mdspan a(v.data(), 2, 3);

    // TRAP: copying an mdspan does NOT copy the data. 'b' is a second view over
    // the SAME buffer, so a write through b is visible through a and through v.
    auto b = a;
    b[0, 0] = 777;
    std::cout << "wrote via b; a[0,0] = " << a[0, 0] << "\n";   // 777
    std::cout << "and v[0]           = " << v[0] << "\n";       // 777

    // FIX: for an independent value, copy the DATA into your own storage and
    // view that. Editing the copy leaves the original untouched.
    std::vector<int> owned(v.begin(), v.end());
    std::mdspan c(owned.data(), 2, 3);
    c[0, 0] = -1;
    std::cout << "after editing the copy: v[0] = " << v[0]
              << ", owned[0] = " << owned[0] << "\n";           // 777, -1
}
