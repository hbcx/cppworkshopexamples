#include <iostream>
#include <string>

// A default member initializer gives a data member a value right in the class
// body. Any constructor that does not set the member uses that value; a
// constructor that DOES set it overrides the default. Members are initialized in
// DECLARATION order, not in the order the constructor's init list lists them.

class Connection {
public:
    Connection() = default;                   // uses all the defaults below
    explicit Connection(int p) : port_(p) {}  // overrides port_, keeps the rest

    void print() const {
        std::cout << host_ << ":" << port_
                  << " retries=" << retries_
                  << " secure=" << (secure_ ? "yes" : "no") << "\n";
    }
private:
    std::string host_ = "localhost";   // default member initializers
    int         port_ = 8080;
    int         retries_{3};            // the {} form works here too
    bool        secure_ = false;
};

// Declaration order controls init order. Here second_ is built from first_, so
// first_ must be declared FIRST -- otherwise it would be read before it exists.
class Ordered {
public:
    Ordered() : first_(10), second_(first_ + 1) {}
    void print() const {
        std::cout << "first=" << first_ << " second=" << second_ << "\n";
    }
private:
    int first_;
    int second_;
};

int main() {
    Connection def;             // all defaults
    def.print();                // localhost:8080 retries=3 secure=no

    Connection custom(9000);    // port overridden, rest defaulted
    custom.print();             // localhost:9000 retries=3 secure=no

    Ordered o;
    o.print();                  // first=10 second=11
    return 0;
}
