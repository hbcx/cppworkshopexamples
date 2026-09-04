#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};

    // ANTI-PATTERN: advancing an iterator past end(). The iterator operations do
    // NOT bounds-check: moving beyond end() gives an iterator that is invalid to
    // dereference, and stepping further is undefined behaviour:
    //
    //   auto it = v.begin();
    //   std::advance(it, 10);   // walks past end() -- undefined behaviour
    //   int bad = *it;          // undefined behaviour: out-of-range dereference
    //
    // Nothing warns you; the fix is to check how far end() is before you move.

    std::size_t wanted = 10;
    auto begin = v.begin();
    auto avail = static_cast<std::size_t>(std::distance(begin, v.end()));

    // FIX: clamp the step to the distance actually available.
    std::size_t step = wanted < avail ? wanted : avail;
    auto it = std::next(begin, static_cast<std::ptrdiff_t>(step));
    if (it != v.end())
        std::cout << "element at " << step << " = " << *it << "\n";
    else
        std::cout << "requested index " << wanted << " but only " << avail
                  << " elements; landed on end()\n";
    return 0;
}
