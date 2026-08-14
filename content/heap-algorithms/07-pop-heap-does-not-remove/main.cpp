#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{9, 6, 4, 3, 1};
    std::make_heap(v.begin(), v.end());
    std::cout << "before: size " << v.size() << ", top " << v.front() << '\n';

    // ANTI-PATTERN: calling pop_heap and expecting the element to be gone.
    // pop_heap only MOVES the maximum to the last position and re-heapifies the
    // rest -- it does not change the container's size. The "removed" 9 is still
    // there, now sitting at the back.
    std::pop_heap(v.begin(), v.end());
    std::cout << "after pop_heap only: size " << v.size()
              << ", back " << v.back()
              << "   <- 9 is still in the container\n";

    // FIX: follow pop_heap with pop_back to actually discard the element.
    v.pop_back();
    std::cout << "after pop_back: size " << v.size()
              << ", new top " << v.front() << '\n';

    // The two always go together: pop_heap to surface the max at the back, then
    // pop_back to remove it. (std::priority_queue::pop does both for you.)
    return 0;
}
