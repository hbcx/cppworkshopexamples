#include <mdspan>
#include <vector>
#include <iostream>
#include <cstddef>

// A custom accessor: reading an element returns twice its stored value.
// The accessor decides HOW an element is fetched from the data handle; the
// default one returns a reference, this one returns a scaled copy by value.
struct scaling_accessor {
    using element_type     = int;
    using data_handle_type = int*;
    using reference        = int;               // a value, not a real reference
    using offset_policy    = scaling_accessor;

    reference access(data_handle_type p, std::size_t i) const { return p[i] * 2; }
    data_handle_type offset(data_handle_type p, std::size_t i) const { return p + i; }
};

int main() {
    std::vector<int> buf{1, 2, 3, 4, 5, 6};

    // Same data, two views. The default accessor reads the stored value;
    // the scaling accessor doubles it on the way out.
    std::mdspan<int, std::dextents<std::size_t, 2>> plain(buf.data(), 2, 3);
    std::mdspan<int, std::dextents<std::size_t, 2>, std::layout_right, scaling_accessor>
        scaled(buf.data(), 2, 3);

    std::cout << "plain [1,2] = " << plain[1, 2] << "\n";    // 6
    std::cout << "scaled[1,2] = " << scaled[1, 2] << "\n";   // 12 (6 * 2)
    std::cout << "buffer is untouched: buf[5] = " << buf[5] << "\n";  // 6
}
