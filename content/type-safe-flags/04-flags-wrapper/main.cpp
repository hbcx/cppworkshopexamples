#include <iostream>
#include <type_traits>

enum class Style : unsigned {
    Bold      = 1u << 0,
    Italic    = 1u << 1,
    Underline = 1u << 2,
    Strike    = 1u << 3,
};

// A small value type that wraps the bits of a flag enum E in one object, giving
// named operations and an explicit bool, instead of raw bitwise operators at
// every call site.
template <class E>
class Flags {
    using U = std::underlying_type_t<E>;
    U bits_ = 0;
public:
    constexpr Flags() = default;
    constexpr Flags(E e) : bits_(static_cast<U>(e)) {}   // build from a single flag

    constexpr Flags& set(E e)   { bits_ |=  static_cast<U>(e); return *this; }
    constexpr Flags& reset(E e) { bits_ &= ~static_cast<U>(e); return *this; }
    constexpr Flags& flip(E e)  { bits_ ^=  static_cast<U>(e); return *this; }

    constexpr bool test(E e) const { return (bits_ & static_cast<U>(e)) == static_cast<U>(e); }
    constexpr bool any()  const { return bits_ != 0; }
    constexpr bool none() const { return bits_ == 0; }
    constexpr int  count() const {                       // number of set bits
        int n = 0;
        for (U b = bits_; b != 0; b &= b - 1) ++n;
        return n;
    }
    constexpr U raw() const { return bits_; }
    constexpr explicit operator bool() const { return bits_ != 0; }

    // Combine a Flags with one more flag. Found by ADL through Flags, which is
    // why the left side is a Flags (a bare Style on the left would not find it).
    friend constexpr Flags operator|(Flags a, E b) { return a.set(b); }
};

int main() {
    Flags<Style> f;
    f.set(Style::Bold).set(Style::Underline).set(Style::Italic);   // chained

    std::cout << "count set: " << f.count() << '\n';                 // 3
    std::cout << "is bold:   " << f.test(Style::Bold) << '\n';       // 1
    std::cout << "is strike: " << f.test(Style::Strike) << '\n';     // 0

    f.reset(Style::Bold);
    std::cout << "after reset bold, is bold: " << f.test(Style::Bold) << '\n';  // 0

    // operator| builds a Flags from a Flags and a single flag.
    Flags<Style> g = Flags<Style>{Style::Bold} | Style::Underline;
    std::cout << "g count: " << g.count() << '\n';                   // 2

    if (f) std::cout << "f still non-empty, raw bits = " << f.raw() << '\n';    // 6 (Italic|Underline)
    Flags<Style> empty;
    std::cout << "empty.none(): " << empty.none() << '\n';           // 1
    return 0;
}
