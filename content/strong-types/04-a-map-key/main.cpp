#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

template <typename T, typename Tag>
class Strong {
public:
    explicit Strong(T v) : value_(v) {}
    const T& get() const { return value_; }
    bool operator==(const Strong& other) const { return value_ == other.value_; }
private:
    T value_;
};

struct UserTag;
using UserId = Strong<int, UserTag>;

// To use a strong type as a key in an unordered_map you need two things: an
// operator== (above) and a hash. The clean way to give it a hash is to
// specialise std::hash for the type and forward to the wrapped value's hash --
// so callers just write unordered_map<UserId, V> with no extra hasher argument.
namespace std {
template <typename T, typename Tag>
struct hash<Strong<T, Tag>> {
    size_t operator()(const Strong<T, Tag>& s) const {
        return std::hash<T>{}(s.get());
    }
};
}  // namespace std

int main() {
    std::unordered_map<UserId, std::string> names;
    names.emplace(UserId{1}, "Ada");
    names.emplace(UserId{2}, "Linus");

    std::cout << "user 1 is " << names.at(UserId{1}) << '\n';
    std::cout << "user 2 is " << names.at(UserId{2}) << '\n';
    std::cout << "map size: " << names.size() << '\n';
    return 0;
}
