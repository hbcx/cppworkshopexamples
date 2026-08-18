#include <iostream>

// A tempting shortcut: give the wrapper every arithmetic operator once, so all
// strong types "just work". The cost is that operations that make no sense for
// the concept now compile silently -- the type stops describing what is legal.
template <typename T, typename Tag>
struct Strong {
    explicit Strong(T v) : value(v) {}
    T value;
    Strong operator+(Strong o) const { return Strong{value + o.value}; }
    Strong operator*(Strong o) const { return Strong{value * o.value}; }
};

struct MoneyTag;
struct UserTag;
using Money = Strong<double, MoneyTag>;
using UserId = Strong<int, UserTag>;

int main() {
    Money price{10.0}, tax{2.0};
    std::cout << "price + tax = " << (price + tax).value << '\n';   // this one is meaningful

    // Nonsense that the blanket operators wave through:
    Money squared = price * tax;    // money times money = money-squared, not money
    std::cout << "money * money compiles (it should not): " << squared.value << '\n';

    UserId a{1}, b{2};
    UserId sum = a + b;             // adding two user ids has no meaning
    std::cout << "userId + userId compiles (it should not): " << sum.value << '\n';

    // The fix is opt-in operators (see the opt-in example): add only the
    // operations each concept should have -- Money gets +, UserId gets none.
    return 0;
}
