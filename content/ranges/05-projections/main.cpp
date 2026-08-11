#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

struct Person {
    std::string name;
    int age;
};

int main() {
    std::vector<Person> people = {
        {"Ann", 30}, {"Bob", 25}, {"Cy", 40}, {"Dee", 35}
    };

    // A projection tells the algorithm which FIELD to work on -- no comparator
    // lambda. The {} leaves the comparison at its default less-than.
    std::ranges::sort(people, {}, &Person::age);
    std::cout << "by age:";
    for (const auto& p : people) std::cout << " " << p.name << "(" << p.age << ")";
    std::cout << "\n";

    // The same projection applies to the min/max family and to find.
    auto oldest = std::ranges::max_element(people, {}, &Person::age);
    std::cout << "oldest: " << oldest->name << "\n";

    auto it = std::ranges::find(people, std::string("Cy"), &Person::name);
    std::cout << "found by name: " << it->name << " age " << it->age << "\n";
    return 0;
}
