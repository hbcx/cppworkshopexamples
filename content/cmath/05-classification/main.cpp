#include <cmath>
#include <iostream>
#include <limits>

const char* classify(double x) {
    switch (std::fpclassify(x)) {
        case FP_INFINITE:  return "infinite";
        case FP_NAN:       return "nan";
        case FP_ZERO:      return "zero";
        case FP_SUBNORMAL: return "subnormal";
        case FP_NORMAL:    return "normal";
        default:           return "unknown";
    }
}

int main() {
    const double inf   = std::numeric_limits<double>::infinity();
    const double nan   = std::numeric_limits<double>::quiet_NaN();
    const double small = std::numeric_limits<double>::denorm_min();  // subnormal

    std::cout << std::boolalpha;
    std::cout << "isnan(nan)    = " << std::isnan(nan)     << "\n";
    std::cout << "isinf(inf)    = " << std::isinf(inf)     << "\n";
    std::cout << "isfinite(1.0) = " << std::isfinite(1.0)  << "\n";
    std::cout << "isnormal(sub) = " << std::isnormal(small)<< "   (subnormal is not normal)\n";

    std::cout << "\nfpclassify:\n";
    std::cout << "  1.0   -> " << classify(1.0)   << "\n";
    std::cout << "  inf   -> " << classify(inf)   << "\n";
    std::cout << "  nan   -> " << classify(nan)   << "\n";
    std::cout << "  small -> " << classify(small) << "\n";

    // signbit sees the sign bit even of a zero, so it tells -0.0 from +0.0 --
    // something == cannot do. copysign moves a sign onto a magnitude.
    std::cout << "\nsigned zero:\n";
    std::cout << "  (-0.0 == 0.0)      = " << (-0.0 == 0.0)        << "\n";
    std::cout << "  signbit(-0.0)      = " << std::signbit(-0.0)   << "\n";
    std::cout << "  copysign(3, -1)    = " << std::copysign(3.0, -1.0) << "\n";
    return 0;
}
