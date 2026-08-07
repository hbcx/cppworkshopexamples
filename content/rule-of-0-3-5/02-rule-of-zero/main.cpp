#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Rule of zero: every member manages itself -- std::string owns its text,
// std::vector owns its buffer, std::shared_ptr owns its object. So this class
// declares NONE of the five special members, and the compiler-generated copy,
// move and destructor are all correct.
struct Config {
    std::string name;
    std::vector<int> ports;
    std::shared_ptr<int> limit;   // shared, so the class stays copyable
};

int main() {
    Config a;
    a.name = "web";
    a.ports = {80, 443};
    a.limit = std::make_shared<int>(1000);

    Config b = a;                 // implicit copy: deep-copies name and ports
    b.name = "api";
    b.ports.push_back(8080);

    Config c = std::move(a);      // implicit move: steals name and ports cheaply

    std::cout << "b.name=" << b.name << " ports=" << b.ports.size() << "\n";
    std::cout << "c.name=" << c.name << " ports=" << c.ports.size() << "\n";
    std::cout << "a.ports after move=" << a.ports.size() << " (moved-from)\n";
    std::cout << "limit shared by " << c.limit.use_count() << " owners\n";
    return 0;
}
