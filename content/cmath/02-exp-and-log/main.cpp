#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);

    std::cout << "exp(1)      = " << std::exp(1.0)          << "   (e)\n";
    std::cout << "log(e)      = " << std::log(std::exp(1.0))<< "   (natural log)\n";
    std::cout << "log2(8)     = " << std::log2(8.0)         << "\n";
    std::cout << "log10(1000) = " << std::log10(1000.0)     << "\n";

    // expm1(x) = exp(x) - 1 and log1p(x) = log(1 + x), but accurate when x is
    // tiny. Computing exp(x)-1 directly loses precision because exp(x) is close
    // to 1 and the subtraction cancels the leading digits. The inputs are read
    // through volatile so the compiler cannot fold these at compile time.
    volatile double vx = 1e-10;
    double x = vx;
    std::cout << std::setprecision(17);
    std::cout << "\nnear zero (x = 1e-10):\n";
    std::cout << "exp(x) - 1 = " << (std::exp(x) - 1.0) << "\n";
    std::cout << "expm1(x)   = " << std::expm1(x)       << "   (accurate)\n";
    std::cout << "log(1 + x) = " << std::log(1.0 + x)   << "\n";
    std::cout << "log1p(x)   = " << std::log1p(x)       << "   (accurate)\n";
    return 0;
}
