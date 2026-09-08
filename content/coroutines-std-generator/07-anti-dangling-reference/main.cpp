#include <generator>
#include <iostream>
#include <string>
#include <vector>

// ANTI-PATTERN (described, NOT run -- it is undefined behaviour):
//
//   std::generator<const std::string&> each(const std::vector<std::string>& v) {
//       for (const std::string& s : v)
//           co_yield s;                       // yields references INTO v
//   }
//   auto g = each(std::vector<std::string>{"a", "b"});  // the temporary vector
//                                                       // dies at this ;
//   for (const std::string& s : g) ...        // body reads the dead vector -> UB
//
// The generator takes its source by reference, but the source was a temporary
// that is destroyed once the call expression ends -- long before the generator is
// iterated. Every yielded reference then points into freed memory.

// FIX: take the source BY VALUE. It is moved into the coroutine frame and lives
// exactly as long as the generator, so references into it stay valid.
std::generator<const std::string&> each(std::vector<std::string> v) {
    for (const std::string& s : v)
        co_yield s;
}

int main() {
    std::cout << "elements of a temporary source:";
    // Safe now: the temporary vector is moved into the generator's frame.
    for (const std::string& s : each(std::vector<std::string>{"ann", "bob", "cy"}))
        std::cout << " " << s;
    std::cout << "\n";
    return 0;
}
