#include <iostream>

// Compile-time Strategy: the algorithm is a TEMPLATE PARAMETER (a policy), chosen
// when the type is instantiated. There is no virtual call and no std::function --
// the policy's operation is known statically, so the compiler can inline it, and
// each configured Checkout is its own distinct, zero-overhead type.
struct RegularPolicy {
    static int priceFor(int base) { return base; }
};

struct MemberPolicy {
    static int priceFor(int base) { return base - base / 10; }   // 10% off
};

template <class Pricing>
class Checkout {
public:
    int total(int base) const { return Pricing::priceFor(base); }   // resolved at compile time
};

int main() {
    Checkout<RegularPolicy> regular;
    std::cout << "regular: " << regular.total(100) << "\n";   // 100

    Checkout<MemberPolicy> member;
    std::cout << "member:  " << member.total(100) << "\n";    // 90
    return 0;
}
