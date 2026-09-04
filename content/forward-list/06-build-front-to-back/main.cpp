#include <forward_list>
#include <iostream>
#include <vector>

static void print(const std::forward_list<int>& fl, const char* label) {
    std::cout << label;
    for (int x : fl)
        std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::forward_list<int> fl;

    // Seed the tail with before_begin(). insert_after RETURNS the new node, so
    // reusing that as the next tail appends in order, each step O(1).
    auto tail = fl.before_begin();
    for (int v : {10, 20, 30, 40}) {
        tail = fl.insert_after(tail, v);   // append and advance the tail together
    }
    print(fl, "built in order: ");   // 10 20 30 40

    // The range overload appends many at once and returns the LAST inserted,
    // which is the new tail -- here we tack a whole vector onto the end.
    std::vector<int> more{50, 60};
    tail = fl.insert_after(tail, more.begin(), more.end());
    print(fl, "after range append: ");   // 10 20 30 40 50 60

    std::cout << "new tail = " << *tail << '\n';   // 60
    return 0;
}
