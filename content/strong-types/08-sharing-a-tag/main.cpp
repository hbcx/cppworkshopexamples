#include <iostream>

template <typename T, typename Tag>
struct Strong {
    explicit Strong(T v) : value(v) {}
    T value;
};

// The tag is the only thing that makes two strong types distinct. Reuse the
// SAME tag for two different meanings and the specializations become the SAME
// type again -- they mix freely, exactly what the wrapper was meant to stop.
// This happens by copy-paste: duplicate a line and forget to change the tag.
struct IdTag;
using UserId = Strong<int, IdTag>;
using ProductId = Strong<int, IdTag>;   // BUG: same tag -> identical type to UserId

void greetUser(UserId id) { std::cout << "hello user #" << id.value << '\n'; }

int main() {
    ProductId product{99};

    // UserId and ProductId are literally one type here, so handing a product id
    // to a user function compiles -- the tag failed to separate them.
    greetUser(product);

    // The fix: a distinct tag per meaning (struct UserTag; struct ProductTag;),
    // so UserId and ProductId are different types and greetUser(product) fails
    // to compile. A tag exists only to be unique -- never share one.
    return 0;
}
