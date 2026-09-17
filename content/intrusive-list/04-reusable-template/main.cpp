#include <iostream>
#include <string>
#include <utility>

// A reusable intrusive list built on a BASE hook: the element inherits from
// ListHook, so the list recovers the element from a hook pointer with a plain
// static_cast (boost::intrusive offers this as base_hook, plus a member_hook
// variant). A circular sentinel removes every null special case.
struct ListHook {
    ListHook* next = nullptr;
    ListHook* prev = nullptr;
};

template <class T>   // T must derive from ListHook
class IntrusiveList {
    ListHook head_{&head_, &head_};   // sentinel: empty list points at itself
public:
    IntrusiveList() = default;
    IntrusiveList(const IntrusiveList&) = delete;             // self-referential
    IntrusiveList& operator=(const IntrusiveList&) = delete;

    void push_back(T& obj) {
        ListHook* h = &obj;                 // T -> ListHook (base subobject)
        h->prev = head_.prev;
        h->next = &head_;
        head_.prev->next = h;
        head_.prev = h;
    }
    static void unlink(T& obj) {
        ListHook* h = &obj;
        h->prev->next = h->next;
        h->next->prev = h->prev;
        h->next = h->prev = nullptr;
    }
    template <class F>
    void for_each(F f) {
        for (ListHook* p = head_.next; p != &head_; p = p->next)
            f(static_cast<T&>(*p));         // ListHook -> T (base -> derived)
    }
    bool empty() const { return head_.next == &head_; }
};

struct Item : ListHook {
    std::string name;
    explicit Item(std::string n) : name(std::move(n)) {}
};

int main() {
    IntrusiveList<Item> list;
    Item x{"x"}, y{"y"}, z{"z"};
    list.push_back(x);
    list.push_back(y);
    list.push_back(z);

    std::cout << "items:";
    list.for_each([](const Item& i) { std::cout << ' ' << i.name; });
    std::cout << '\n';                                                   // x y z

    IntrusiveList<Item>::unlink(y);   // remove y in O(1), no end cases
    std::cout << "after unlink y:";
    list.for_each([](const Item& i) { std::cout << ' ' << i.name; });
    std::cout << '\n';                                                   // x z
    return 0;
}
