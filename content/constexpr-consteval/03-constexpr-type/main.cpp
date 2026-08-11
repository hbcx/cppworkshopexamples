#include <iostream>

// A "literal type": a class whose constructor and member functions are constexpr,
// so its objects can live in constant expressions and be built by the compiler.
class Fraction {
public:
    constexpr Fraction(int num, int den) : num_(num), den_(den) {}
    constexpr int num() const { return num_; }
    constexpr int den() const { return den_; }
    constexpr Fraction operator*(Fraction other) const {
        return Fraction(num_ * other.num_, den_ * other.den_);
    }
    constexpr double value() const { return static_cast<double>(num_) / den_; }

private:
    int num_;
    int den_;
};

int main() {
    constexpr Fraction half(1, 2);
    constexpr Fraction third(1, 3);
    constexpr Fraction sixth = half * third;      // built at compile time
    static_assert(sixth.num() == 1 && sixth.den() == 6, "sixth must be 1/6");

    std::cout << sixth.num() << "/" << sixth.den() << " = " << sixth.value() << "\n";
    return 0;
}
