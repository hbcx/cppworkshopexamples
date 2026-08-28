#include <iostream>

// Anti-pattern: an implicit operator bool. It makes if (flag) work, but it also
// lets the flag silently become a bool -- and therefore an int -- anywhere, so
// meaningless expressions compile instead of being caught.

class LeakyFlag {
public:
    explicit LeakyFlag(bool s) : set_(s) {}
    operator bool() const { return set_; }     // BAD: implicit conversion to bool
private:
    bool set_;
};

class SafeFlag {
public:
    explicit SafeFlag(bool s) : set_(s) {}
    explicit operator bool() const { return set_; }   // FIX: explicit
private:
    bool set_;
};

int main() {
    LeakyFlag a{true}, b{false};

    // All of these compile because a and b decay to bool, then to int. None of
    // them mean anything -- but the compiler never complains.
    int sum    = a + b;        // true + false -> 1 + 0
    int scaled = a * 10;       // true * 10   -> 10
    bool eq    = (a == b);     // compares the bools, not the flags

    std::cout << "a + b   = " << sum    << "\n";   // 1
    std::cout << "a * 10  = " << scaled << "\n";   // 10
    std::cout << "a == b  = " << eq     << "\n";   // 0

    // SafeFlag still works where a truth value is genuinely wanted...
    SafeFlag s{true};
    if (s) std::cout << "safe flag is set\n";      // printed
    std::cout << "as bool: " << static_cast<bool>(s) << "\n";  // 1

    // ...but "s + 1", "s * 10", "s == other" no longer compile: the explicit
    // keyword blocks exactly the accidental conversions that bit LeakyFlag.
    return 0;
}
