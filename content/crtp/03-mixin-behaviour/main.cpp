// CRTP as a mixin: generate boilerplate from a type's core operations.
//
// Comparable<T> defines !=, <=, > and >= in terms of the T's own == and <. A
// value type inherits Comparable<Self> and writes only == and <, getting all six
// relational operators. The generated operators static_cast to T and inline, so
// there is no runtime cost -- just no copy-paste.

#include <iostream>

// The mixin: four operators derived from the two the derived type provides.
template <class T>
class Comparable {
public:
    bool operator!=(const T& other) const { return !derived().operator==(other); }
    bool operator<=(const T& other) const { return !(other < derived()); }
    bool operator>(const T& other) const { return other < derived(); }
    bool operator>=(const T& other) const { return !(derived() < other); }
private:
    const T& derived() const { return static_cast<const T&>(*this); }
};

// A value type: writes == and <, inherits the rest.
class Version : public Comparable<Version> {
public:
    Version(int major, int minor) : major_(major), minor_(minor) {}
    bool operator==(const Version& o) const { return major_ == o.major_ && minor_ == o.minor_; }
    bool operator<(const Version& o) const {
        return major_ != o.major_ ? major_ < o.major_ : minor_ < o.minor_;
    }
    int major() const { return major_; }
    int minor() const { return minor_; }
private:
    int major_;
    int minor_;
};

static const char* yn(bool b) { return b ? "yes" : "no"; }

int main() {
    Version v1(1, 4);
    Version v2(2, 0);

    // All six operators work, but only == and < were written.
    std::cout << "v1 == v2 : " << yn(v1 == v2) << "\n";
    std::cout << "v1 != v2 : " << yn(v1 != v2) << "\n";
    std::cout << "v1 <  v2 : " << yn(v1 < v2) << "\n";
    std::cout << "v1 <= v2 : " << yn(v1 <= v2) << "\n";
    std::cout << "v1 >  v2 : " << yn(v1 > v2) << "\n";
    std::cout << "v1 >= v2 : " << yn(v1 >= v2) << "\n";
}
