// std::stack adapts a container, chosen by the second template argument. The
// default is std::deque, but a stack only works one end, so a std::vector
// backing (contiguous, cache-friendly) is usually the better pick. Behaviour is
// identical; only the layout differs.

#include <deque>
#include <iostream>
#include <stack>
#include <type_traits>
#include <vector>

int main() {
    std::cout << std::boolalpha;
    std::cout << "default backing is std::deque? "
              << std::is_same<std::stack<int>::container_type, std::deque<int>>::value
              << '\n';

    std::stack<int> defaultStack;                    // deque-backed
    std::stack<int, std::vector<int>> vectorStack;   // vector-backed

    for (int i = 1; i <= 3; ++i) { defaultStack.push(i); vectorStack.push(i); }

    std::cout << "deque-backed top:  " << defaultStack.top() << '\n';
    std::cout << "vector-backed top: " << vectorStack.top() << '\n';
    std::cout << "same LIFO behaviour, vector backing is often faster\n";
}
