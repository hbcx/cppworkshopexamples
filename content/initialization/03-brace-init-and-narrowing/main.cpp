#include <iostream>

// Brace initialization {} forbids "narrowing" conversions -- ones that can lose
// information (double -> int, a value that might not fit a smaller integer).
// The older () and = forms allow them silently. This is the main practical
// reason to prefer {}.

int main() {
    double pi = 3.99;

    // Direct/copy init happily truncate double -> int, losing the fraction:
    int truncated(pi);       // 3, silently
    int alsoTruncated = pi;  // 3, silently
    std::cout << "int(pi)  = " << truncated << "\n";
    std::cout << "int = pi = " << alsoTruncated << "\n";

    // Brace init REJECTS the same conversion at compile time:
    //
    //     int rejected{pi};   // error: narrowing conversion of double to int
    //
    // That is the feature: the loss becomes a compile error you must acknowledge
    // with an explicit cast.
    int acknowledged{static_cast<int>(pi)};   // 3, but you asked for it
    std::cout << "int{(int)pi} = " << acknowledged << "\n";

    // A value that fits is fine in braces -- no narrowing happens:
    int exact{7};
    char c{65};              // 65 fits in char
    std::cout << "int{7}          = " << exact << "\n";
    std::cout << "char{65} as int = " << static_cast<int>(c) << "\n";

    // A constant that does NOT fit is also an error under braces:
    //     char tooBig{300};   // error: 300 does not fit in char
    return 0;
}
