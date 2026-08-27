#include <iostream>
#include <vector>

// Anti-pattern: braces on a container when you meant a size. Because {} prefers
// an initializer_list constructor, std::vector<int>{3} is NOT "three elements"
// -- it is ONE element with value 3. The (parentheses) form is the size
// constructor. Mixing them up builds a container of the wrong length.

int main() {
    // Intended: a vector of three ints. Got: one int equal to 3.
    std::vector<int> braces{3};
    std::cout << "vector<int>{3}   -> size " << braces.size()
              << ", front " << braces.front() << "\n";     // size 1, front 3

    // The size constructor uses parentheses: three value-initialized ints (0).
    std::vector<int> parens(3);
    std::cout << "vector<int>(3)   -> size " << parens.size()
              << ", front " << parens.front() << "\n";     // size 3, front 0

    // Two arguments makes the trap louder:
    std::vector<int> braces2{3, 0};   // two elements: 3 and 0
    std::vector<int> parens2(3, 0);   // three elements: 0, 0, 0
    std::cout << "vector<int>{3, 0} -> size " << braces2.size() << "\n";  // 2
    std::cout << "vector<int>(3, 0) -> size " << parens2.size() << "\n";  // 3

    // The rule: use () when the numbers are a size or fill, use {} when the
    // numbers are the actual elements.
    return 0;
}
