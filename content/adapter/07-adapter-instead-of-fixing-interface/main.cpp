#include <iostream>
#include <string>

// You OWN both of these. The client wants greet(); your class offers sayHello().
// They differ only because you never aligned them.
struct Greeter {                        // the interface the client wants
    virtual std::string greet() const = 0;
    virtual ~Greeter() = default;
};

struct MyServiceOld {                    // your own class, wrong method name
    std::string sayHello() const { return "hi"; }
};

// Anti-pattern: paper over your own mismatch with an adapter. Now there are
// three types where one would do, and every call goes through a pointless
// translation layer you are free to delete.
struct SelfAdapter : Greeter {
    MyServiceOld svc;
    std::string greet() const override { return svc.sayHello(); }
};

// Fix: since you own the class, change it to present the interface directly. No
// adapter, no extra type, no indirection.
struct MyService : Greeter {
    std::string greet() const override { return "hi"; }
};

int main() {
    SelfAdapter adapted;
    std::cout << "adapter (needless) -> " << adapted.greet() << '\n';

    MyService fixed;
    std::cout << "fixed interface    -> " << fixed.greet() << '\n';
    return 0;
}
