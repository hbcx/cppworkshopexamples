#include <iostream>
#include <list>

static void print(const std::list<int>& l, const char* label) {
    std::cout << label;
    for (int x : l) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    std::list<int> l{5, 3, 9, 3, 1, 9, 3, 8, 4};
    print(l, "start:            ");

    // std::sort(l.begin(), l.end());   // does NOT compile: std::sort requires
    //                                  // random-access iterators; a list has
    //                                  // bidirectional ones. Hence the member:
    l.sort();                           // O(n log n), stable, relinks nodes,
    print(l, "after sort():     ");     // and keeps every iterator valid

    // unique() collapses CONSECUTIVE duplicates only -- which is why it is a
    // deduplicator just after a sort, and something else entirely before one.
    auto saved = ++l.begin();           // watch a node survive the whole thing
    l.unique();
    print(l, "after unique():   ");

    // remove() really removes: it unlinks and destroys the matching nodes and
    // the list gets shorter. std::remove cannot do that -- it does not know the
    // container, so it only shifts survivors forward and hands back a new end,
    // which a vector then has to erase(). A list needs no such idiom.
    l.remove(9);
    print(l, "after remove(9):  ");

    l.remove_if([](int x) { return x % 2 == 0; });
    print(l, "after remove_if(even): ");

    std::cout << "the node saved before all of it still reads: " << *saved << '\n';

    // merge() splices a second SORTED list into this one, again without moving
    // any value. The source ends up empty.
    std::list<int> other{2, 4, 6};
    l.merge(other);
    print(l, "after merge:      ");
    std::cout << "source list emptied: " << (other.empty() ? "yes" : "no") << '\n';

    l.reverse();
    print(l, "after reverse():  ");
    return 0;
}
