#include <iostream>

// Meters converts from a plain int on purpose (1 meter, 2 meters...). That single
// design choice is what makes the member-vs-free decision visible: only a FREE
// operator+ lets the left operand be converted, so 2 + m compiles.

class Meters {
public:
    Meters(int value = 0) : value_(value) {}   // intentionally not explicit

    int value() const { return value_; }

    // MEMBER: changes the object, left operand is always a Meters. This is the
    // right home for compound assignment.
    Meters& operator+=(const Meters& rhs) {
        value_ += rhs.value_;
        return *this;
    }

private:
    int value_;
};

// FREE: treats both operands the same. Because a member operator+ would fix the
// left side to be a Meters already, 2 + m could not convert the 2 -- a free
// function can, since both arguments go through the normal conversions.
Meters operator+(Meters lhs, const Meters& rhs) {
    lhs += rhs;
    return lhs;
}

// FREE: the left operand is the stream, so this simply cannot be a member.
std::ostream& operator<<(std::ostream& os, const Meters& m) {
    return os << m.value() << " m";
}

int main() {
    Meters m{10};

    // Both work because operator+ is free and int converts to Meters:
    std::cout << "m + 5 = " << (m + 5) << "\n";   // 15 m
    std::cout << "2 + m = " << (2 + m) << "\n";   // 12 m  (the int on the LEFT converts)

    m += 3;
    std::cout << "after += 3: " << m << "\n";     // 13 m
    return 0;
}
