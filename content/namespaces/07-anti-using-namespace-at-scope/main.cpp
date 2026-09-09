#include <algorithm>
#include <iostream>
#include <vector>

// Our own helper named count.
namespace app {
    int count(const std::vector<int>& v) { return static_cast<int>(v.size()); }
}

// ANTI-PATTERN: pulling whole namespaces in at file scope. Now the unqualified
// name `count` refers to BOTH app::count and std::count, and this only gets worse
// in a header, where it dumps these names into every file that includes it.
using namespace std;
using namespace app;

int main() {
    vector<int> v{1, 2, 2, 3};

    // Writing `count(v)` unqualified here is AMBIGUOUS -- app::count vs std::count --
    // and fails to compile. With the pollution in place you are forced to qualify
    // anyway, which is what you should have done from the start:
    cout << "app::count(v)        = " << app::count(v) << "\n";
    cout << "std::count(...,2)    = " << std::count(v.begin(), v.end(), 2) << "\n";
    return 0;
}
