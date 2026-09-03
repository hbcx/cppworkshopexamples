#include <functional>
#include <iostream>
#include <string>

// A free FUNCTION TEMPLATE. std::bind cannot deduce a template argument, so you
// bind a NAMED instantiation (transform<int>). Once fixed, callers never repeat
// the type -- and the bound argument is gone from the call too.
template <typename T>
T transform(T scale, T value) { return scale * value; }

// A class with a plain member function...
struct Discount {
    int percent;
    long apply(long price) const { return price - price * percent / 100; }
};

// ...a class with a MEMBER FUNCTION TEMPLATE...
struct Printer {
    std::string prefix;
    template <typename T>
    std::string show(const T& value) const { return prefix + std::to_string(value); }
};

// ...and a CLASS TEMPLATE whose member is itself a FUNCTION TEMPLATE.
template <typename Key>
struct Cache {
    std::string name;
    template <typename Value>
    std::string put(Key key, Value value) const {
        return name + ": " + std::to_string(key) + " -> " + std::to_string(value);
    }
};

int main() {
    using namespace std::placeholders;

    // Bind scale = 3 and pick the int instantiation. The result takes ONE int;
    // you no longer pass the scale, nor spell transform<int>, at each call.
    auto triple = std::bind(transform<int>, 3, _1);
    std::cout << "triple(14) = " << triple(14) << "\n";              // 42

    // The same template, a different instantiation, bound for doubles.
    auto half = std::bind(transform<double>, 0.5, _1);
    std::cout << "half(9.0)  = " << half(9.0) << "\n";               // 4.5

    // Bind a class instance to its member function: the object is fixed, so the
    // callable takes only the price -- the Discount is not passed each time.
    Discount tenOff{10};
    auto applyTenOff = std::bind(&Discount::apply, &tenOff, _1);
    std::cout << "applyTenOff(200) = " << applyTenOff(200) << "\n";  // 180

    // Both at once -- a member function TEMPLATE: name the instantiation
    // (show<int>) and bind the object; only the value is left to pass.
    Printer p{"value="};
    auto showInt = std::bind(&Printer::show<int>, &p, _1);
    std::cout << showInt(42) << "\n";                                // value=42

    // The fullest case: a member function template of a CLASS template. Name
    // BOTH template arguments -- the class (Cache<int>) and the method
    // (put<long>) -- and bind the object. The call site is left with only the
    // runtime key and value; no template arguments appear there at all.
    Cache<int> cache{"cache"};
    auto put = std::bind(&Cache<int>::put<long>, &cache, _1, _2);
    std::cout << "put(7, 300) = " << put(7, 300) << "\n";            // cache: 7 -> 300
    return 0;
}
