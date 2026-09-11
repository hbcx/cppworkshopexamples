#include <iostream>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

struct Cat {};
struct Dog {};
struct Bird {};

int main() {
    // std::type_info is not copyable and cannot be a map key on its own.
    // std::type_index wraps it into a copyable, hashable, comparable value, so a
    // type can key a map -- the basis of a type-to-handler registry.
    std::unordered_map<std::type_index, std::string> sounds;
    sounds[typeid(Cat)]  = "meow";
    sounds[typeid(Dog)]  = "woof";
    sounds[typeid(Bird)] = "tweet";

    std::cout << "Cat  -> " << sounds[typeid(Cat)] << "\n";
    std::cout << "Dog  -> " << sounds[typeid(Dog)] << "\n";
    std::cout << "Bird -> " << sounds[typeid(Bird)] << "\n";
    std::cout << "registered types: " << sounds.size() << "\n";
    return 0;
}
