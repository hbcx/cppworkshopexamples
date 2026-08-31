// default_construct vs value_construct in raw storage.
//
// For a CLASS TYPE with a user default constructor, both algorithms call it --
// no difference. The split matters only for TRIVIAL types (int, plain structs):
//   - value_construct  -> zero-initializes
//   - default_construct -> leaves the bytes indeterminate (does not touch them)

#include <iostream>
#include <memory>   // uninitialized_default_construct, uninitialized_value_construct, destroy
#include <string>

struct Loud {
    Loud() { std::cout << "  Loud() ran\n"; }
};

int main() {
    // 1) Class type with a default constructor: default_construct calls it.
    const std::size_t n = 3;
    Loud* loud = static_cast<Loud*>(::operator new(sizeof(Loud) * n));
    std::cout << "default_construct on a class type:\n";
    std::uninitialized_default_construct(loud, loud + n);
    std::destroy(loud, loud + n);
    ::operator delete(loud);

    // 2) Trivial type: value_construct zero-initializes every int.
    int* nums = static_cast<int*>(::operator new(sizeof(int) * n));
    std::uninitialized_value_construct(nums, nums + n);
    std::cout << "value_construct on ints gave:";
    for (std::size_t i = 0; i < n; ++i) std::cout << ' ' << nums[i];
    std::cout << '\n';
    std::destroy(nums, nums + n);   // trivial: a no-op, but kept for symmetry
    ::operator delete(nums);

    // NOTE: default_construct on those same ints would leave them indeterminate
    // -- reading them expecting 0 is a bug, so we do not read them. That is the
    // only difference between the two algorithms.
    std::cout << "(default_construct would leave ints indeterminate)\n";
}
