#include <functional>
#include <iostream>
#include <string>

int scale(int value, int factor) { return value * factor; }

int main() {
    using namespace std::placeholders;

    // ANTI-PATTERN: reaching for std::bind where a lambda says the same thing
    // more clearly. bind hides the argument wiring behind _1/_2 placeholders,
    // needs the placeholders namespace, and reads backwards once there is more
    // than one fixed argument.
    auto tripleBind = std::bind(scale, _1, 3);
    std::cout << "bind:   " << tripleBind(14) << "\n";    // 42

    // FIX: a lambda. The body shows exactly what is called and with what; there
    // are no placeholders to decode, and the compiler can inline it -- bind
    // returns an opaque object that is harder to optimize and to read.
    auto tripleLambda = [](int v){ return scale(v, 3); };
    std::cout << "lambda: " << tripleLambda(14) << "\n";  // 42

    // The gap widens with captures and members: bind needs std::ref and careful
    // placeholder counting, while a lambda's capture list is explicit. Modern
    // guidance (and the C++ Core Guidelines) is to prefer the lambda; keep bind
    // for maintaining older code and the rare very terse partial application.
    std::string tag = "id=";
    auto label = [tag](int n){ return tag + std::to_string(n); };
    std::cout << "label:  " << label(7) << "\n";          // id=7
    return 0;
}
