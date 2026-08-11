#include <iostream>

struct Widget {
    // A member FUNCTION template.
    template <int N>
    int scaled() const { return N * 10; }

    // A member CLASS template (a nested template).
    template <class T>
    struct Slot { T value; };
};

// .template : call a member template on a dependent object. Without the keyword
// the compiler reads `scaled < 5` as a less-than, because it does not yet know
// scaled is a template (w's type is a template parameter).
template <class W>
int viaDot(const W& w) {
    return w.template scaled<5>();
}

// ->template : the same thing through a pointer.
template <class W>
int viaArrow(const W* w) {
    return w->template scaled<7>();
}

// ::template : name a dependent NESTED template. This needs BOTH keywords --
// typename because Slot<int> is a type, and template because Slot is a template.
template <class W>
int viaScope() {
    typename W::template Slot<int> s{42};
    return s.value;
}

int main() {
    Widget w;
    std::cout << "w.template scaled<5>()  = " << viaDot(w) << "\n";       // 50
    std::cout << "w->template scaled<7>() = " << viaArrow(&w) << "\n";    // 70
    std::cout << "W::template Slot<int>   = " << viaScope<Widget>() << "\n"; // 42
    return 0;
}
