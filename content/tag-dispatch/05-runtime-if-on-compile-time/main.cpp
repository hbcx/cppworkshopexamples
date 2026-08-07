#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// We want: for a class type, print its size(); for a number, print the value.
// Tag dispatch instantiates ONLY the chosen overload, so each body need only be
// valid for the types that actually reach it.
template <class T>
void describe(const T& x, std::true_type)  { std::cout << "container, size=" << x.size() << "\n"; }
template <class T>
void describe(const T& x, std::false_type) { std::cout << "value=" << x << "\n"; }

// Dispatcher: std::is_class stands in for a real has-size trait.
template <class T>
void describe(const T& x) { describe(x, std::is_class<T>{}); }

int main() {
    describe(std::vector<int>{1, 2, 3});   // class -> size path
    describe(std::string("hi"));           // class -> size path
    describe(42);                          // non-class -> value path

    // THE ANTI-PATTERN (does not compile, shown here as a comment):
    //   template <class T> void describeBad(const T& x) {
    //       if (std::is_class<T>::value) std::cout << x.size();   // needs .size()
    //       else                         std::cout << x;          // needs operator<<
    //   }
    // For T = int the compiler STILL compiles the x.size() branch even though it
    // is never taken at run time -- and int has no size(), so the build fails.
    // A run-time if must compile BOTH branches for every T; tag dispatch compiles
    // only the branch selected for that T.
    return 0;
}
