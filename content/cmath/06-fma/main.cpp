#include <cmath>
#include <iostream>

int main() {
    // fma(a, b, c) computes a*b + c with a SINGLE rounding at the end: the full
    // exact product a*b is formed before c is added, so no bits are lost between
    // the multiply and the add. The naive a*b + c rounds a*b first.
    //
    // Pick a = b = 100000001 (1e8 + 1). The exact product is
    //   a*b = 10000000200000001,
    // which is just above 1e16, where a double's step (ULP) is 2 -- so the
    // trailing "+1" cannot be represented and a*b rounds down to
    //   10000000200000000.
    // Adding c = -10000000200000000 then gives 0, hiding the +1 entirely.
    // fma keeps the full product and recovers the 1. volatile blocks any
    // compile-time folding so we measure the run-time operations.
    volatile double va = 100000001.0;      // 1e8 + 1
    volatile double vb = 100000001.0;
    volatile double vc = -10000000200000000.0;   // -(1e16 + 2e8)
    double a = va, b = vb, c = vc;

    double naive = a * b + c;
    double fused = std::fma(a, b, c);

    std::cout << "naive  a*b + c = " << naive << "   (the +1 was rounded away)\n";
    std::cout << "fma(a, b, c)   = " << fused << "   (full product kept)\n";
    std::cout << "\nBoth compute a*b + c, but the naive form rounds the product\n"
                 "to a double before adding c, losing the low bit; fma rounds once.\n";
    return 0;
}
