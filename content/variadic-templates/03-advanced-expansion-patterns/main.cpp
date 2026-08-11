#include <cstddef>
#include <iostream>
#include <utility>

// A pattern that mentions TWO packs expands them in lockstep. Here an index pack
// Is and the value pack args expand together, so each argument gets its position.
template <class... Ts, std::size_t... Is>
void printNumbered(std::index_sequence<Is...>, Ts... args) {
    // pattern uses BOTH Is and args -> "#0: a", "#1: b", ...
    (void)std::initializer_list<int>{
        (std::cout << "  #" << Is << ": " << args << "\n", 0)...
    };
}

template <class... Ts>
void printNumbered(Ts... args) {
    // index_sequence_for<Ts...> is 0, 1, ..., N-1, sized from the pack.
    printNumbered(std::index_sequence_for<Ts...>{}, args...);
}

int main() {
    printNumbered("alpha", 42, 3.14);
    return 0;
}
