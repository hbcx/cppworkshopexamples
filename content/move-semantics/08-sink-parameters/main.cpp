#include <iostream>
#include <string>
#include <utility>

// A "sink" is a function or constructor that STORES its argument. The question
// is how to take that argument so both lvalues and rvalues are handled cheaply.
struct Resource {
    std::string label;
    explicit Resource(std::string l) : label(std::move(l)) { std::cout << "ctor(" << label << ")\n"; }
    Resource(const Resource& o) : label(o.label) { std::cout << "copy(" << label << ")\n"; }
    Resource(Resource&& o) noexcept : label(std::move(o.label)) { std::cout << "move\n"; }
};

// Approach 1: const& / && overloads. Optimal (1 copy for lvalue, 1 move for
// rvalue), but you need 2^N overloads for N parameters, and a mixed call falls
// through to the const& overload -- an avoidable copy.
struct ByOverload {
    Resource r;
    ByOverload(const Resource& res) : r(res) {}
    ByOverload(Resource&& res) : r(std::move(res)) {}
};

// Approach 2: pass by value + move. One constructor for every value category.
// Costs exactly one extra move vs overloads for an lvalue; identical for an
// rvalue (the prvalue is elided straight into the parameter).
struct ByValue {
    Resource r;
    explicit ByValue(Resource res) : r(std::move(res)) {}
};

// Approach 3: forwarding-reference template. Zero overhead for any value
// category, but it is a template: worse error messages, it greedily matches
// everything, and in real code it needs constraining.
struct ByForward {
    Resource r;
    template <typename T>
    explicit ByForward(T&& res) : r(std::forward<T>(res)) {}
};

int main() {
    Resource lvalue("lv");

    // Each object exists only for its constructor's side effect (the printed
    // trace); [[maybe_unused]] says so and silences -Wunused-variable.
    std::cout << "--- ByOverload(lvalue) -> 1 copy ---\n";
    [[maybe_unused]] ByOverload o1(lvalue);
    std::cout << "--- ByOverload(rvalue) -> 1 move ---\n";
    [[maybe_unused]] ByOverload o2(Resource("rv"));

    std::cout << "--- ByValue(lvalue) -> 1 copy + 1 extra move ---\n";
    [[maybe_unused]] ByValue v1(lvalue);
    std::cout << "--- ByValue(rvalue) -> 1 move (same as overload) ---\n";
    [[maybe_unused]] ByValue v2(Resource("rv"));

    std::cout << "--- ByForward(lvalue) -> 1 copy ---\n";
    [[maybe_unused]] ByForward f1(lvalue);
    std::cout << "--- ByForward(rvalue) -> 1 move ---\n";
    [[maybe_unused]] ByForward f2(Resource("rv"));

    std::cout << "--- end ---\n";
    return 0;
}
