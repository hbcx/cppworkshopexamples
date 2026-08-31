// std::queue and std::stack are adaptors over a container, and both default to
// std::deque -- it supplies the cheap end operations they expose. The
// container_type typedef confirms the default.

#include <deque>
#include <iostream>
#include <queue>
#include <stack>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;
    std::cout << "std::queue<int> is backed by std::deque<int>? "
              << std::is_same<std::queue<int>::container_type, std::deque<int>>::value << '\n';
    std::cout << "std::stack<int> is backed by std::deque<int>? "
              << std::is_same<std::stack<int>::container_type, std::deque<int>>::value << '\n';

    std::queue<int> q;                 // FIFO
    q.push(1); q.push(2); q.push(3);
    std::cout << "queue pops (FIFO):";
    while (!q.empty()) { std::cout << ' ' << q.front(); q.pop(); }
    std::cout << '\n';

    std::stack<int> s;                 // LIFO
    s.push(1); s.push(2); s.push(3);
    std::cout << "stack pops (LIFO):";
    while (!s.empty()) { std::cout << ' ' << s.top(); s.pop(); }
    std::cout << '\n';
}
