#include <iostream>
#include <string>

// A class template: the class is parameterised on a type. Box<int> and
// Box<std::string> are DIFFERENT types generated from one definition.
template <class T>
class Box {
public:
    explicit Box(T value) : value_(value) {}
    const T& get() const { return value_; }
    void set(T value) { value_ = value; }

private:
    T value_;
};

int main() {
    Box<int> bi(42);
    Box<std::string> bs("hello");
    std::cout << "Box<int>    = " << bi.get() << "\n";
    std::cout << "Box<string> = " << bs.get() << "\n";

    bi.set(100);
    std::cout << "after set   = " << bi.get() << "\n";
    return 0;
}
