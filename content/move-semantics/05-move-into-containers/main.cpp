#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::string> v;
    v.reserve(4);  // avoid reallocations while we push

    std::string s = "hello";

    v.push_back(s);             // copies s -- s is an lvalue and stays usable
    v.push_back(std::move(s));  // moves s into the vector -- s is now empty

    v.emplace_back("built in place");  // constructs the string directly in the vector

    std::cout << "size=" << v.size() << " front=" << v.front() << '\n';
    std::cout << "s after move is empty? " << (s.empty() ? "yes" : "no") << '\n';
    return 0;
}
