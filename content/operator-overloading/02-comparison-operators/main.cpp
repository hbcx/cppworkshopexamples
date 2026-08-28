#include <iostream>

// A version number, ordered major then minor. We want all six comparisons, but
// we only WRITE two of them (== and <) and derive the rest, so they can never
// disagree with each other.

class Version {
public:
    Version(int major, int minor) : major_(major), minor_(minor) {}

    // The two primitives, as free friends so both sides convert alike.
    friend bool operator==(const Version& a, const Version& b) {
        return a.major_ == b.major_ && a.minor_ == b.minor_;
    }
    friend bool operator<(const Version& a, const Version& b) {
        if (a.major_ != b.major_) return a.major_ < b.major_;
        return a.minor_ < b.minor_;
    }

    // The other four are defined purely in terms of == and <.
    friend bool operator!=(const Version& a, const Version& b) { return !(a == b); }
    friend bool operator>(const Version& a, const Version& b)  { return b < a; }
    friend bool operator<=(const Version& a, const Version& b) { return !(b < a); }
    friend bool operator>=(const Version& a, const Version& b) { return !(a < b); }

private:
    int major_, minor_;
};

int main() {
    Version v1{1, 4};
    Version v2{1, 10};
    Version v3{1, 4};

    std::cout << "v1 == v3? " << (v1 == v3) << "\n";   // 1
    std::cout << "v1 != v2? " << (v1 != v2) << "\n";   // 1
    std::cout << "v1 <  v2? " << (v1 < v2)  << "\n";   // 1  (1.4 before 1.10)
    std::cout << "v2 >  v1? " << (v2 > v1)  << "\n";   // 1
    std::cout << "v1 <= v3? " << (v1 <= v3) << "\n";   // 1
    std::cout << "v2 >= v3? " << (v2 >= v3) << "\n";   // 1
    return 0;
}
