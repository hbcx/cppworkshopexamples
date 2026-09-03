#include <functional>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> data{1, 2, 3};

    // A reference_wrapper holds a reference, so it dangles exactly when a raw
    // reference or pointer would: if the referent dies or moves, the wrapper is
    // left pointing at freed memory. A classic way to trigger it: wrap elements
    // of a vector, then make that vector REALLOCATE.
    std::vector<std::reference_wrapper<int>> refs;
    for (int& x : data) refs.push_back(std::ref(x));

    // push_back may reallocate data's storage, moving every element to a new
    // address. Every wrapper in refs would then refer to freed memory -- reading
    // refs[0].get() after this would be UNDEFINED BEHAVIOUR, so we do not do it.
    data.push_back(4);   // <-- invalidates the wrappers in refs if it reallocated

    // FIX: reserve capacity up front so no reallocation happens while the
    // wrappers are alive (or refer to storage with stable addresses -- std::list,
    // std::deque, or objects that outlive the wrappers).
    std::vector<int> safe;
    safe.reserve(8);
    safe.insert(safe.end(), {1, 2, 3});
    std::vector<std::reference_wrapper<int>> safeRefs;
    for (int& x : safe) safeRefs.push_back(std::ref(x));
    safe.push_back(4);   // capacity 8, no reallocation -> wrappers stay valid
    std::cout << "safeRefs[0] = " << safeRefs[0].get() << "\n";   // 1
    std::cout << "safeRefs count = " << safeRefs.size() << "\n";  // 3
    return 0;
}
