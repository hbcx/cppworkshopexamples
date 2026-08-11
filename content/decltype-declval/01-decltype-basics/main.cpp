#include <iostream>
#include <vector>

int main() {
    int a = 5;
    double b = 2.5;

    // decltype(expr) is the TYPE of the expression, worked out without evaluating
    // it. Declare a variable of the same type as another expression.
    decltype(a) x = 10;            // int
    decltype(a + b) y = a + b;     // double (int + double -> double)

    std::vector<int> v = {1, 2, 3};
    decltype(v.size()) n = v.size();   // the container's own size_type, whatever it is

    std::cout << "x=" << x << " y=" << y << " n=" << n << "\n";

    // decltype does NOT evaluate its operand: this names std::ostream& but prints
    // nothing, because the insertion is never actually performed.
    decltype(std::cout << "not printed") streamRef = std::cout;
    (void) streamRef;

    std::cout << "done\n";
    return 0;
}
