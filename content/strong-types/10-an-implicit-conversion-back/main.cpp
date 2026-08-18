#include <iostream>

// This wrapper adds an implicit conversion back to the raw value "for
// convenience". It quietly undoes the safety: wherever a raw int is accepted,
// the strong type decays to one on its own, so wrong-type mixing compiles again
// -- the value leaks out through the front door instead of an explicit get().
template <typename Tag>
struct Leaky {
    explicit Leaky(int v) : value(v) {}
    operator int() const { return value; }   // implicit decay -- the leak
    int value;
};

struct UserTag;
struct ProductTag;
using UserId = Leaky<UserTag>;
using ProductId = Leaky<ProductTag>;

int priceOf(int productId) { return productId * 10; }

int main() {
    UserId user{3};

    // priceOf wants a product id (a raw int). UserId decays to int on its own,
    // so a USER id is accepted and returns a wrong-but-defined answer:
    std::cout << "user id slipped through as int, price = " << priceOf(user) << '\n';   // 30

    // Two unrelated strong types now compare equal, because both decay to int
    // before the comparison -- no operator== needed, and none intended:
    ProductId product{3};
    std::cout << "unrelated types compare equal via int: "
              << std::boolalpha << (user == product) << '\n';   // true

    // The fix: drop the implicit operator int and expose the value only through
    // an explicit get(). Then neither the wrong-id call nor the cross-type
    // comparison compiles -- the raw value never escapes on its own.
    return 0;
}
