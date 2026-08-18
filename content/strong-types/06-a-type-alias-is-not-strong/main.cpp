#include <iostream>

// A type alias creates a NEW NAME for an existing type, not a new type. So both
// of these "ids" are just int, and they mix freely -- the alias buys a readable
// name in the signature, but zero safety. This is the most common thing people
// reach for and think they have made a strong type.
using UserId = int;
using ProductId = int;

int priceOf(ProductId product);   // wants a PRODUCT id...

int priceOf(ProductId product) {
    return product * 10;           // pretend lookup: product 7 costs 70
}

int main() {
    UserId user = 3;
    ProductId product = 7;

    // BUG: the user id is passed where a product id is wanted. Both are int, so
    // it compiles and quietly returns a wrong-but-defined answer -- no warning.
    std::cout << "wrong id (user 3) accepted, price = " << priceOf(user) << '\n';    // 30
    std::cout << "correct id (product 7),   price = " << priceOf(product) << '\n';   // 70

    // The fix is a real wrapper type -- Strong<int, UserTag> vs
    // Strong<int, ProductTag> (see the first example). With those, priceOf(user)
    // would not compile, so the mistake is caught instead of shipped.
    return 0;
}
