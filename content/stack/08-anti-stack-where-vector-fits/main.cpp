// Anti-pattern: using std::stack when you also need to see the elements.
//
// A vector is a stack too: push_back = push, back = top, pop_back = pop. But it
// ALSO iterates and indexes, which std::stack blocks. Use stack for the
// discipline, vector when the same code must scan or index the contents.

#include <iostream>
#include <vector>

int main() {
    // A vector used as a stack.
    std::vector<int> st;
    st.push_back(1);
    st.push_back(2);
    st.push_back(3);          // "top" is st.back()

    std::cout << "top: " << st.back() << '\n';

    // The extra reach a std::stack would deny you: inspect the whole stack.
    std::cout << "full contents (bottom to top):";
    for (int x : st) std::cout << ' ' << x;
    std::cout << '\n';

    st.pop_back();            // "pop"
    std::cout << "after pop, top: " << st.back() << '\n';
}
