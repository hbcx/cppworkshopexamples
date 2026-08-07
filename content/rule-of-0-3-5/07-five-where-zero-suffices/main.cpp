#include <iostream>
#include <string>
#include <utility>

// BAD: all members are self-managing (a std::string and an int), so rule of
// zero would give a correct copy for free. Someone wrote the copy by hand
// anyway -- and when the tag_ member was added later, they forgot it. The bug
// is invisible until you copy.
class BadWidget {
public:
    BadWidget(std::string name, int tag) : name_(std::move(name)), tag_(tag) {}
    BadWidget(const BadWidget& o) : name_(o.name_) {}                 // forgot tag_
    BadWidget& operator=(const BadWidget& o) { name_ = o.name_; return *this; }  // forgot tag_
    std::string name_;
    int tag_ = 0;
};

// GOOD: rule of zero. Declare nothing; the compiler copies EVERY member,
// including one added later.
class GoodWidget {
public:
    GoodWidget(std::string name, int tag) : name_(std::move(name)), tag_(tag) {}
    std::string name_;
    int tag_ = 0;
};

int main() {
    BadWidget a("a", 42);
    BadWidget b = a;
    std::cout << "bad copy:  name=" << b.name_ << " tag=" << b.tag_ << "  <- tag lost\n";

    GoodWidget c("c", 42);
    GoodWidget d = c;
    std::cout << "good copy: name=" << d.name_ << " tag=" << d.tag_ << "  <- tag kept\n";
    return 0;
}
