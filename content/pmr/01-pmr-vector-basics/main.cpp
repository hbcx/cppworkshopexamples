// std::pmr::vector<int> is std::vector<int, std::pmr::polymorphic_allocator<int>>:
// one type whatever the memory strategy. With no resource it uses the default,
// which is new_delete_resource() -- so it acts just like std::vector until you
// give it another resource.

#include <iostream>
#include <memory_resource>
#include <vector>

int main() {
    std::pmr::vector<int> v;   // default resource == new/delete
    for (int i = 1; i <= 5; ++i) v.push_back(i * i);

    std::cout << "contents:";
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';

    // The vector's resource is the new/delete one by default.
    bool isDefault = v.get_allocator().resource() == std::pmr::new_delete_resource();
    std::cout << "using new_delete_resource by default? "
              << std::boolalpha << isDefault << '\n';
}
