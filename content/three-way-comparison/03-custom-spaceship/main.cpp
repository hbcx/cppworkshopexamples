#include <iostream>
#include <compare>
#include <string>

// Employees are identified and ordered by their id alone -- name and salary do not
// take part in the order. A defaulted <=> would compare all three, so we write it
// by hand and forward to the id only.

struct Employee {
    int id;
    std::string name;
    long salary;

    // Order by id only. int <=> int is strong_ordering, so that is our category.
    std::strong_ordering operator<=>(const Employee& other) const {
        return id <=> other.id;
    }

    // A user-written <=> does NOT give us ==, and we want == to match the order:
    // same id means the same employee, whatever the other fields say.
    bool operator==(const Employee& other) const {
        return id == other.id;
    }
};

int main() {
    Employee a{7, "Ada", 9000};
    Employee b{7, "Bob", 4000};   // same id, different name and salary
    Employee c{9, "Cid", 9000};

    // Equal by id, even though name and salary differ:
    std::cout << "a == b? " << (a == b) << "\n";   // 1
    std::cout << "a <  c? " << (a < c)  << "\n";   // 1  (id 7 before 9)
    std::cout << "c >  b? " << (c > b)  << "\n";   // 1
    return 0;
}
