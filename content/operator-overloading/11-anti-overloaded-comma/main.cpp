#include <iostream>

// Anti-pattern: overloading operator, . The built-in comma "a, b" evaluates a,
// then b, and the expression's value is b. Overloading it silently changes that
// contract for your type.

struct Tag {
    const char* name;
};

// BAD: this comma yields the LEFT operand -- the opposite of the built-in rule.
// It also turns "left, right" into a function call, dropping the guarantee that
// left is fully evaluated before right.
Tag operator,(Tag a, Tag /*b*/) { return a; }

int main() {
    // Built-in comma on ints: left runs (side effect), the value is the RIGHT
    // operand. This is what every reader expects a comma to do.
    int counter = 0;
    int r = (++counter, 42);
    std::cout << "built-in comma: value=" << r
              << ", counter=" << counter << "\n";   // value=42, counter=1

    // Overloaded comma on Tag: same syntax, but now the expression yields the
    // LEFT operand. Nothing at the call site hints that "left, right" means left.
    Tag left{"left"};
    Tag right{"right"};
    Tag picked = (left, right);
    std::cout << "overloaded comma yields: " << picked.name << "\n";  // left

    // THE FIX: do not overload comma. If you wanted to combine things, name it:
    //   seq.add(left).add(right);
    // so the reader sees exactly what happens and the built-in comma keeps its
    // meaning everywhere else.
    return 0;
}
