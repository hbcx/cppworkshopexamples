#include <concepts>
#include <iostream>
#include <string>
#include <utility>

template <class T>
concept Printable = requires(const T& x, std::ostream& os) {
    { os << x } -> std::same_as<std::ostream&>;
};

// A class template with a member that EXISTS only when T is Printable. The rest
// of Box works for any T.
template <class T>
class Box {
public:
    explicit Box(T v) : value_(std::move(v)) {}
    const T& get() const { return value_; }

    void print() const requires Printable<T> {
        std::cout << "Box(" << value_ << ")\n";
    }

private:
    T value_;
};

// A class template whose partial specialization is chosen by a concept: integral
// types get the compact form, everything else the primary.
template <class T>
struct Kind { static std::string name() { return "generic"; } };
template <std::integral T>
struct Kind<T> { static std::string name() { return "integral"; } };

struct Opaque {};   // no operator<<

int main() {
    Box<int> a(7);
    a.print();                      // ok: int is Printable

    Box<Opaque> b(Opaque{});        // builds and works...
    (void) b.get();
    // b.print();                   // ...but this would not compile: no print() for Opaque
    std::cout << "Box<Opaque> built and used without print()\n";

    std::cout << "Kind<int>    = " << Kind<int>::name() << "\n";      // integral
    std::cout << "Kind<double> = " << Kind<double>::name() << "\n";   // generic
    std::cout << "done\n";
}
