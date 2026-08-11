#include <iostream>
#include <type_traits>

struct Widget {
    int data;
    void run() {}
};

int main() {
    std::cout << std::boolalpha;

    // Plain pointers:
    static_assert(std::is_pointer_v<int*> && !std::is_pointer_v<int>);
    static_assert(std::is_same_v<std::remove_pointer_t<int*>, int>);
    static_assert(std::is_same_v<std::remove_pointer_t<int**>, int*>);   // peels one level
    static_assert(std::is_same_v<std::add_pointer_t<int>, int*>);

    // Pointers to members are a SEPARATE category -- NOT is_pointer.
    using DataPtr = int Widget::*;
    using FuncPtr = void (Widget::*)();
    static_assert(!std::is_pointer_v<DataPtr>);                     // a member pointer is not a plain pointer
    static_assert(std::is_member_pointer_v<DataPtr>);
    static_assert(std::is_member_object_pointer_v<DataPtr>);        // points to a data member
    static_assert(std::is_member_function_pointer_v<FuncPtr>);      // points to a member function
    static_assert(!std::is_member_object_pointer_v<FuncPtr>);

    std::cout << "pointer and member-pointer traits hold\n";
    return 0;
}
