#include <iostream>
#include <type_traits>

enum class Level : unsigned char { Low, High };

int main() {
    // Transformation traits return a modified TYPE via ::type / the _t helper.
    static_assert(std::is_same_v<std::remove_reference_t<int&>,  int>);
    static_assert(std::is_same_v<std::remove_reference_t<int&&>, int>);
    static_assert(std::is_same_v<std::remove_cv_t<const volatile int>, int>);
    static_assert(std::is_same_v<std::add_pointer_t<int>, int*>);
    static_assert(std::is_same_v<std::make_unsigned_t<int>, unsigned int>);

    // decay mimics what happens to a by-value parameter: strip ref and cv, and
    // turn arrays and functions into pointers.
    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
    static_assert(std::is_same_v<std::decay_t<int[5]>, int*>);

    // underlying_type gives the integer type an enum is stored as.
    static_assert(std::is_same_v<std::underlying_type_t<Level>, unsigned char>);

    std::cout << "type transformations hold\n";
    return 0;
}
