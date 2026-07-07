#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Item {
    std::string name;
    Item(const char* n) : name(n) { std::cout << "ctor(" << name << ")\n"; }   // implicit on purpose
    Item(const Item& o) : name(o.name) { std::cout << "copy(" << name << ")\n"; }
    Item(Item&& o) noexcept : name(std::move(o.name)) { std::cout << "move\n"; }
    ~Item() { std::cout << "dtor(" << name << ")\n"; }
};

int main() {
    std::vector<Item> v;
    v.reserve(8);   // no reallocation for the rest of the demo -- isolate the calls

    std::cout << "--- push_back(lvalue): ALWAYS a copy ---\n";
    Item a("A");
    v.push_back(a);              // a is an lvalue -> copy, even though Item is movable

    std::cout << "--- push_back(std::move(lvalue)): a move ---\n";
    v.push_back(std::move(a));   // now an rvalue -> move

    std::cout << "--- push_back(rvalue): ctor + move + dtor(temp) ---\n";
    v.push_back(Item("B"));      // build temp, move into vector, destroy temp

    std::cout << "--- emplace_back(lvalue): STILL a copy ---\n";
    Item c("C");
    v.emplace_back(c);           // emplace does not turn an lvalue into an rvalue

    std::cout << "--- emplace_back(ctor args): builds in place, no temp ---\n";
    v.emplace_back("D");         // one ctor directly in the slot; push_back needs ctor+move+dtor

    std::cout << "--- emplace_back trap: forwards to EXPLICIT ctors ---\n";
    std::vector<std::vector<int>> nested;
    nested.emplace_back(3);      // calls explicit vector(size_t): a vector of 3 ints, not value 3
    std::cout << "nested[0].size() = " << nested[0].size() << "\n";

    std::cout << "--- end ---\n";
    return 0;
}
