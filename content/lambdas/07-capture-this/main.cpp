#include <iostream>
#include <string>

// A lambda in a member function can reach the object four ways, along two axes:
// SHARE the object (a reference/pointer -- sees changes, dangles if the object
// dies) versus COPY it (own snapshot, safe to outlive), and the WHOLE object
// versus just ONE member.

struct Greeter {
    std::string name = "Ada";

    // SHARE, whole object: [this] captures the object pointer.
    auto byThis() {
        return [this] { return "hi " + name; };
    }
    // SHARE, one member: [&n = name] binds a reference to just this->name. It does
    // not store this, but the reference still points into the object, so it has the
    // SAME lifetime risk as [this] -- it dangles if the Greeter dies.
    auto byMemberRef() {
        return [&n = name] { return "hi " + n; };
    }
    // COPY, whole object: [*this] (C++17) copies the entire Greeter into the closure.
    auto byStarThis() {
        return [*this] { return "hi " + name; };
    }
    // COPY, one member: [n = name] (init-capture) copies just this->name. The
    // cheapest way that is still safe to outlive the object.
    auto byMember() {
        return [n = name] { return "hi " + n; };
    }
};

int main() {
    Greeter g;
    auto a = g.byThis();       // shares the object
    auto b = g.byMemberRef();  // shares just name
    auto c = g.byStarThis();   // copy of the whole object
    auto d = g.byMember();     // copy of just name

    // Change the original. The two SHARING closures follow it; the two COPIES do not.
    g.name = "Grace";
    std::cout << "byThis:      " << a() << '\n';   // hi Grace
    std::cout << "byMemberRef: " << b() << '\n';   // hi Grace
    std::cout << "byStarThis:  " << c() << '\n';   // hi Ada
    std::cout << "byMember:    " << d() << '\n';   // hi Ada
    return 0;
}
