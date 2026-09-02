#include <cmath>
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::setprecision(6);
    const double pi = std::acos(-1.0);   // a portable pi, no magic constant

    std::cout << "sin(pi/6) = " << std::sin(pi / 6) << "   (0.5)\n";
    std::cout << "cos(pi/3) = " << std::cos(pi / 3) << "   (0.5)\n";
    std::cout << "tan(pi/4) = " << std::tan(pi / 4) << "   (1.0)\n";

    // atan2(y, x) returns the angle of the point (x, y) over the FULL circle
    // (-pi, pi], choosing the quadrant from the two signs. atan(y/x) cannot:
    // the division collapses the signs, so it never leaves (-pi/2, pi/2).
    std::cout << "\nangle of a 2D vector (radians):\n";
    std::cout << "atan2( 1,  1) = " << std::atan2( 1.0,  1.0) << "   ( 45 deg)\n";
    std::cout << "atan2( 1, -1) = " << std::atan2( 1.0, -1.0) << "   (135 deg)\n";
    std::cout << "atan2(-1, -1) = " << std::atan2(-1.0, -1.0) << "   (-135 deg)\n";

    std::cout << "\natan cannot tell the quadrants apart:\n";
    std::cout << "atan( 1 /  1) = " << std::atan( 1.0 /  1.0) << "\n";
    std::cout << "atan(-1 / -1) = " << std::atan(-1.0 / -1.0) << "   (same value!)\n";
    return 0;
}
