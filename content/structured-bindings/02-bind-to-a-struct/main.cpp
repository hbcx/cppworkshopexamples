#include <iostream>
#include <string>

// Structured bindings also unpack a plain struct (an aggregate): each name binds
// to one data member, in declaration order. You must name EVERY public member,
// no more and no fewer.

struct Point3D {
    double x;
    double y;
    double z;
};

struct Employee {
    std::string name;
    int         age;
    double      salary;
};

int main() {
    Point3D p{1.5, 2.0, -3.0};
    auto [x, y, z] = p;          // three members -> three names, in order
    std::cout << "x=" << x << " y=" << y << " z=" << z << "\n";

    Employee e{"bob", 30, 5500.0};
    auto [name, age, salary] = e;
    std::cout << name << " is " << age << ", earns " << salary << "\n";

    // The names follow the member ORDER, not the member names -- you pick the
    // local names, but they map positionally.
    return 0;
}
