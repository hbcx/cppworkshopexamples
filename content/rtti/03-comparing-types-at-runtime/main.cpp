#include <iostream>
#include <typeinfo>

int main() {
    const std::type_info& ti_int = typeid(int);
    const std::type_info& ti_dbl = typeid(double);

    std::cout << std::boolalpha;
    std::cout << "int == int?    " << (ti_int == typeid(int)) << "\n";
    std::cout << "int != double? " << (ti_int != ti_dbl) << "\n";

    // before() imposes a strict total order over types. Its direction is
    // implementation-defined, but it is consistent: for two different types,
    // exactly one of before(a,b) / before(b,a) is true.
    bool ab = ti_int.before(ti_dbl);
    bool ba = ti_dbl.before(ti_int);
    std::cout << "before() gives a strict order (exactly one direction): " << (ab != ba) << "\n";

    // hash_code() returns a size_t; equal types always hash equal.
    std::cout << "typeid(int) hashes equal to itself? "
              << (typeid(int).hash_code() == ti_int.hash_code()) << "\n";
    return 0;
}
