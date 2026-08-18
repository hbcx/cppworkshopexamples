#include <iostream>

// The base holds the value and nothing else -- no operators at all. That is the
// point: a bare Strong supports only construction and get(), so no operation
// sneaks in that does not make sense for the concept the type stands for.
template <typename T, typename Tag>
class Strong {
public:
    explicit Strong(T v) : value_(v) {}
    const T& get() const { return value_; }
private:
    T value_;
};

// A mixin adds ONE capability. It is a CRTP base templated on the concrete
// strong type (Self), so it can build and read a Self. This is CRTP done right:
// the template parameter is the DERIVED type, not the wrapped value type -- the
// mistake in many hand-rolled strong types is passing the value type here.
template <typename Self>
struct Addable {
    friend Self operator+(const Self& a, const Self& b) {
        return Self{a.get() + b.get()};
    }
};

template <typename Self>
struct Printable {
    friend std::ostream& operator<<(std::ostream& os, const Self& s) {
        return os << s.get();
    }
};

template <typename Self>
struct EqualityComparable {
    friend bool operator==(const Self& a, const Self& b) { return a.get() == b.get(); }
    friend bool operator!=(const Self& a, const Self& b) { return !(a == b); }
};

// Money can be added and printed, but is deliberately NOT ordered or multiplied.
struct MoneyTag;
struct Money : Strong<double, MoneyTag>, Addable<Money>, Printable<Money> {
    using Strong::Strong;
};

// A user id can be compared for equality and printed, but adding two user ids
// is meaningless -- so Addable is simply left off its list.
struct UserTag;
struct UserId : Strong<int, UserTag>, EqualityComparable<UserId>, Printable<UserId> {
    using Strong::Strong;
};

int main() {
    Money a{10.50}, b{4.25};
    std::cout << "sum of money: " << (a + b) << '\n';        // Addable + Printable

    UserId u1{7}, u2{7};
    std::cout << "same user: " << std::boolalpha << (u1 == u2) << '\n';

    // Money bad = a * b;    // no operator* -- money times money is nonsense
    // UserId s = u1 + u2;   // no operator+ for UserId -- adding ids is nonsense
    return 0;
}
