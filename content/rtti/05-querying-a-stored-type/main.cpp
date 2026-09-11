#include <any>
#include <iostream>
#include <string>
#include <typeinfo>

// std::any stores a value of any type and erases it. any::type() gives back the
// type_info of what is inside, so you can check before casting it out -- avoiding
// the std::bad_any_cast a wrong any_cast would throw.
void describe(const std::any& a) {
    if (a.type() == typeid(int))
        std::cout << "int:    " << std::any_cast<int>(a) << "\n";
    else if (a.type() == typeid(std::string))
        std::cout << "string: " << std::any_cast<std::string>(a) << "\n";
    else
        std::cout << "unhandled type\n";
}

int main() {
    describe(std::any{42});
    describe(std::any{std::string("hello")});
    describe(std::any{3.14});
    return 0;
}
