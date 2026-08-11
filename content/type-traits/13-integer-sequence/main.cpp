#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

// std::index_sequence<Is...> carries a compile-time pack of indices. Taking it as
// a parameter lets the compiler DEDUCE Is..., which we then expand.
template <class Tuple, std::size_t... Is>
void printTuple(const Tuple& t, std::index_sequence<Is...>) {
    // fold over the pack: print each element indexed by Is, comma-separated
    ((std::cout << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
}

template <class... Ts>
void printTuple(const std::tuple<Ts...>& t) {
    std::cout << "(";
    printTuple(t, std::make_index_sequence<sizeof...(Ts)>{});   // 0, 1, ..., N-1
    std::cout << ")\n";
}

int main() {
    printTuple(std::make_tuple(1, "two", 3.5));   // (1, two, 3.5)

    // index_sequence is an alias for integer_sequence<size_t, ...>.
    static_assert(std::is_same_v<std::make_index_sequence<3>,
                                 std::index_sequence<0, 1, 2>>);

    std::cout << "done\n";
    return 0;
}
