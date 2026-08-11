#include <iostream>
#include <type_traits>

struct Empty {};
struct WithData { int x; };
struct Poly { virtual ~Poly() = default; };
struct Abstract { virtual void f() = 0; };
struct Final final {};

int main() {
    std::cout << std::boolalpha;

    // Qualifier properties (top-level cv):
    static_assert(std::is_const_v<const int> && !std::is_const_v<int>);
    static_assert(std::is_volatile_v<volatile int>);

    // Signedness (meaningful for arithmetic types):
    static_assert(std::is_signed_v<int> && !std::is_signed_v<unsigned>);
    static_assert(std::is_unsigned_v<unsigned>);

    // Class-shape properties:
    static_assert(std::is_empty_v<Empty> && !std::is_empty_v<WithData>);              // no data members
    static_assert(std::is_polymorphic_v<Poly> && !std::is_polymorphic_v<WithData>);   // has a virtual
    static_assert(std::is_abstract_v<Abstract> && !std::is_abstract_v<Poly>);         // has a pure virtual
    static_assert(std::is_final_v<Final> && !std::is_final_v<Empty>);
    static_assert(std::has_virtual_destructor_v<Poly>);

    // Layout / triviality (the two independent properties is_pod bundled):
    static_assert(std::is_standard_layout_v<WithData>);
    static_assert(std::is_trivial_v<WithData>);

    std::cout << "type-property traits hold\n";
    return 0;
}
