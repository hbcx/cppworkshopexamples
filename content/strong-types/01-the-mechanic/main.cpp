#include <iostream>

// A "strong type" wraps a value in a distinct type marked by a phantom Tag.
// The Tag carries no data and appears in no member -- it exists only so the
// compiler treats Strong<int, UserTag> and Strong<int, ProductTag> as two
// unrelated types, even though both hold an int.
template <typename T, typename Tag>
class Strong {
public:
    explicit Strong(T value) : value_(value) {}
    const T& get() const { return value_; }
private:
    T value_;
};

// Each tag is just a type used as a label. Declaring them as distinct (even
// incomplete) structs is enough -- they are never instantiated.
struct UserTag;
struct ProductTag;
using UserId = Strong<int, UserTag>;
using ProductId = Strong<int, ProductTag>;

// The function wants a user id, and nothing else can slip in.
void greetUser(UserId id) {
    std::cout << "Hello, user #" << id.get() << '\n';
}

int main() {
    UserId user{42};
    ProductId product{42};

    greetUser(user);           // fine
    // greetUser(product);     // COMPILE ERROR: ProductId is not a UserId,
                               // even though both wrap the int 42.
    // greetUser(42);          // COMPILE ERROR: the ctor is explicit, so a raw
                               // int does not convert to a UserId on its own.

    std::cout << "raw values are equal: " << std::boolalpha
              << (user.get() == product.get())
              << ", but the two types never mix\n";
    return 0;
}
