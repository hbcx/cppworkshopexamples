#include <cstddef>
#include <iostream>

// Two independent properties. A type can satisfy both.
template <class T> concept Small    = sizeof(T) <= 4;
template <class T> concept Aligned4 = alignof(T) >= 4;

// GOOD: one concept built FROM the other, so it SUBSUMES it. For a type that is
// both, the more-refined overload is chosen unambiguously.
template <class T> concept Fast   = Small<T>;
template <class T> concept Faster = Small<T> && Aligned4<T>;

template <Fast T>   void run(const T&) { std::cout << "fast\n"; }
template <Faster T> void run(const T&) { std::cout << "faster\n"; }

int main() {
    run(int{});      // int is Small and Aligned4 -> Faster subsumes Fast
    run(char{});     // char is Small but not Aligned4 -> Fast
    std::cout << "done\n";

    // THE ANTI-PATTERN: had run() been overloaded on the two INDEPENDENT
    // concepts instead --
    //     template <Small T>    void run(const T&);
    //     template <Aligned4 T> void run(const T&);
    // -- then run(int{}) would be AMBIGUOUS: int satisfies both, and neither
    // constraint is built from the other, so neither subsumes and the compiler
    // cannot choose. Subsumption orders overloads only when one concept is
    // defined in terms of the other, as Faster is from Small above.
}
