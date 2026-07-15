#include <iostream>
#include <set>
#include <string>

struct Employee {
    int id;
    std::string name;
};

// Order -- and therefore uniqueness -- is by id only: two employees with the same
// id are the "same" element as far as the set is concerned.
struct ById {
    bool operator()(const Employee& a, const Employee& b) const {
        return a.id < b.id;
    }
};

int main() {
    std::set<Employee, ById> staff;
    staff.insert({1, "Alice"});
    staff.insert({2, "Bob"});
    auto dup = staff.insert({1, "Impostor"});   // same id as Alice -> rejected

    std::cout << "second insert of id 1 succeeded? " << (dup.second ? "yes" : "no") << '\n';
    std::cout << "staff (ordered by id):\n";
    for (const auto& e : staff) {
        std::cout << "  " << e.id << " = " << e.name << '\n';
    }

    // Looking up by id needs a probe object carrying that id.
    Employee probe{2, ""};
    auto it = staff.find(probe);
    std::cout << "find id 2 -> " << (it != staff.end() ? it->name : "none") << '\n';
    return 0;
}
