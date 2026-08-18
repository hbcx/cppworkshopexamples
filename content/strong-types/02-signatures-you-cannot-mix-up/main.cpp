#include <iostream>

template <typename T, typename Tag>
class Strong {
public:
    explicit Strong(T v) : value_(v) {}
    const T& get() const { return value_; }
private:
    T value_;
};

struct MoneyTag;
struct AccountTag;
using Money = Strong<double, MoneyTag>;
using AccountId = Strong<long, AccountTag>;

// The raw version would be: void charge(double amount, long account).
// There, charge(account, amount) still compiles -- both arguments are numbers,
// so a swap is a silent bug that ships. With strong types the wrong order
// simply does not compile, because Money and AccountId are unrelated types.
void charge(Money amount, AccountId account) {
    std::cout << "charging " << amount.get()
              << " to account " << account.get() << '\n';
}

int main() {
    Money amount{19.99};
    AccountId account{100234};

    charge(amount, account);        // correct order
    // charge(account, amount);     // COMPILE ERROR: arguments cannot be swapped
    return 0;
}
