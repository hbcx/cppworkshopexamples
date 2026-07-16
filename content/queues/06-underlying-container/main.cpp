#include <deque>
#include <iostream>
#include <list>
#include <queue>
#include <type_traits>

template <typename Q>
static void drain(Q q, const char* label) {
    std::cout << label;
    while (!q.empty()) {
        std::cout << ' ' << q.front();
        q.pop();
    }
    std::cout << '\n';
}

int main() {
    // The default: std::queue<T> IS std::queue<T, std::deque<T>>.
    static_assert(std::is_same<std::queue<int>::container_type, std::deque<int>>::value,
                  "the default underlying container is std::deque");

    std::queue<int> byDeque;
    byDeque.push(1);
    byDeque.push(2);
    drain(byDeque, "deque-backed:");

    // std::list also has front/back/push_back/pop_front, so it qualifies. Worth it
    // only when elements must never move; it costs an allocation per node.
    std::queue<int, std::list<int>> byList;
    byList.push(3);
    byList.push(4);
    drain(byList, "list-backed: ");

    // std::vector can NEVER back a queue: it has no pop_front. The trap is that
    // the declaration itself compiles -- members are instantiated only when used:
    //   std::queue<int, std::vector<int>> broken;   // fine
    //   broken.push(1);                             // still fine (push_back exists)
    //   broken.pop();                               // ERROR here: no pop_front

    // A queue can also be seeded from a ready-made container, instead of a push loop.
    std::deque<int> initial{7, 8, 9};
    std::queue<int> seeded(initial);
    drain(seeded, "seeded:      ");
    return 0;
}
