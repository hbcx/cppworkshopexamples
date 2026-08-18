#include <iostream>

// This wrapper looks strong -- distinct type, a phantom tag -- but its
// constructor is NOT explicit. That single missing keyword lets any raw int
// turn into a UserId on its own, so the wrapper stops forcing callers to say
// what a number means, and half the safety quietly leaks away.
template <typename Tag>
struct Weak {
    Weak(int v) : value(v) {}   // missing: explicit
    int value;
};

struct UserTag;
using UserId = Weak<UserTag>;

void banUser(UserId id) { std::cout << "banned user #" << id.value << '\n'; }

int main() {
    // Because the constructor is implicit, a bare int silently becomes a UserId.
    // The whole point of the wrapper -- making the caller be explicit about what
    // the number is -- is lost. This compiles when it should not:
    banUser(500);

    // The fix is one word: mark the constructor `explicit`. Then banUser(500)
    // fails to compile and the caller must write the intent: banUser(UserId{500}).
    UserId u{7};
    banUser(u);
    return 0;
}
