#include <iostream>
#include <string>
#include <type_traits>

// A trait users can extend. By default a type is NOT pod-like.
template <class T>
struct is_pod_like : std::false_type {};

struct Point { int x; int y; };

// Opt Point into the fast path by specializing the trait -- one line, next to
// the type, without modifying Point itself.
template <>
struct is_pod_like<Point> : std::true_type {};

// Fast path for opted-in types.
template <class T>
void save(const T& v, std::true_type) {
    std::cout << "binary save of " << sizeof(T) << " bytes\n";
    (void)v;
}
// Generic path for everything else.
template <class T>
void save(const T& v, std::false_type) {
    std::cout << "generic save: " << v << "\n";
}
// Dispatcher: build the tag from the trait.
template <class T>
void save(const T& v) {
    save(v, is_pod_like<T>{});
}

int main() {
    save(Point{3, 4});             // opted in -> binary path
    save(std::string("hello"));    // not opted in -> generic path
    save(42);                      // not opted in -> generic path
    return 0;
}
