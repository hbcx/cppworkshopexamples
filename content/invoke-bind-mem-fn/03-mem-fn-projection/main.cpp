#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Employee {
    std::string name;
    int salary;
    bool active() const { return salary > 0; }
};

int main() {
    std::vector<Employee> staff{
        {"Ada", 5000}, {"Ben", 0}, {"Cy", 7000}, {"Dan", 3000}};

    // std::mem_fn turns a pointer-to-member into a callable object you can hand
    // straight to an algorithm. Project each salary into another vector.
    std::vector<int> salaries;
    std::transform(staff.begin(), staff.end(), std::back_inserter(salaries),
                   std::mem_fn(&Employee::salary));
    std::cout << "salaries:";
    for (int s : salaries) std::cout << " " << s;
    std::cout << "\n";

    // It wraps member FUNCTIONS too: count everyone whose active() is true.
    auto activeCount = std::count_if(staff.begin(), staff.end(),
                                     std::mem_fn(&Employee::active));
    std::cout << "active: " << activeCount << "\n";   // 3

    // The equivalent lambda is just as short and reads more directly, which is
    // why lambdas usually win. mem_fn earns its place when the pointer-to-member
    // is a value handed to you and you want to call it like a plain functor.
    auto byLambda = [](const Employee& e){ return e.salary; };
    std::cout << "first via lambda: " << byLambda(staff.front()) << "\n";   // 5000
    return 0;
}
