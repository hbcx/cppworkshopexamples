#include <iostream>
#include <vector>

void takeThree(int a, int b, int c) {
    std::cout << "takeThree: " << a << " " << b << " " << c << "\n";
}

int square(int x) { return x * x; }

template <class... Ts>
void demo(Ts... args) {
    // 1) Expand the pack as call arguments: args... becomes a, b, c.
    takeThree(args...);

    // 2) Expand with a PATTERN applied to each: square(args)... becomes
    //    square(a), square(b), square(c), used to build a vector.
    std::vector<int> squares = { square(args)... };
    std::cout << "squares:";
    for (int s : squares) std::cout << " " << s;
    std::cout << "\n";
}

int main() {
    demo(2, 3, 4);   // takeThree: 2 3 4 ; squares: 4 9 16
    return 0;
}
