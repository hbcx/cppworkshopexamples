// Anti-pattern: uninitialized_default_construct on trivial types, then reading
// the values as if they were zero.
//
// For a trivial type (int), default-initialization does nothing -- the bytes
// keep whatever junk was there. value_construct is the one that zero-initializes.

#include <iostream>
#include <memory>   // uninitialized_default_construct, uninitialized_value_construct, destroy

int main() {
    const std::size_t n = 4;

    // BAD (indeterminate -- described, not read):
    //   int* p = static_cast<int*>(::operator new(sizeof(int) * n));
    //   std::uninitialized_default_construct(p, p + n);
    //   // p[i] is now indeterminate; using it expecting 0 is a bug, and
    //   // reading an indeterminate value is undefined. We do not read it.

    // GOOD: value_construct zero-initializes trivial types.
    int* nums = static_cast<int*>(::operator new(sizeof(int) * n));
    std::uninitialized_value_construct(nums, nums + n);

    std::cout << "value_construct gave:";
    for (std::size_t i = 0; i < n; ++i) std::cout << ' ' << nums[i];
    std::cout << '\n';

    std::destroy(nums, nums + n);
    ::operator delete(nums);
}
