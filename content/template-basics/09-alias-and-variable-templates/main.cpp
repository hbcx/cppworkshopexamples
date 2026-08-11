#include <iostream>
#include <map>
#include <string>

// An ALIAS template names a family of types. StringMap<V> IS
// std::map<std::string, V> -- one alias instead of spelling the map out.
template <class V>
using StringMap = std::map<std::string, V>;

// A VARIABLE template (C++14) is a family of constants, one per type.
template <class T>
constexpr T pi = T(3.1415926535897932385L);

int main() {
    StringMap<int> ages;   // = std::map<std::string, int>
    ages["ann"] = 30;
    ages["bob"] = 25;
    std::cout << "ann is " << ages["ann"] << "\n";

    std::cout << "pi<float>  = " << pi<float> << "\n";     // the constant as a float
    std::cout << "pi<double> = " << pi<double> << "\n";    // the constant as a double
    return 0;
}
