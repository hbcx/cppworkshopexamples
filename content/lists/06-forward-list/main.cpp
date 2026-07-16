#include <forward_list>
#include <iostream>
#include <iterator>
#include <string>

static void print(const std::forward_list<std::string>& fl, const char* label) {
    std::cout << label;
    for (const std::string& s : fl) {
        std::cout << ' ' << s;
    }
    std::cout << '\n';
}

int main() {
    std::forward_list<std::string> chain{"beta", "gamma"};

    // Only the head is cheap: there is no push_back, because reaching the tail
    // would mean walking every node.
    chain.push_front("alpha");
    print(chain, "chain:  ");

    // Nothing points backwards, so you cannot insert BEFORE a position -- every
    // mutating operation is named *_after and takes the node in front of it.
    auto first = chain.begin();                  // -> alpha
    chain.insert_after(first, "alpha-and-a-half");
    print(chain, "after insert_after(first): ");

    // To touch the head itself you need a position BEFORE it, which is what the
    // phantom before_begin() iterator is for. It is not dereferenceable.
    chain.insert_after(chain.before_begin(), "start");
    print(chain, "after insert_after(before_begin()): ");

    chain.erase_after(chain.before_begin());     // drop the head again
    print(chain, "after erase_after(before_begin()): ");

    // No size(): keeping a counter would cost a word per list and, worse, would
    // make splice_after O(n) instead of O(1). Count it yourself if you must --
    // and notice that this walks the whole chain.
    std::cout << "length (std::distance, O(n)): "
              << std::distance(chain.begin(), chain.end()) << '\n';

    // What it buys: one pointer per node instead of two, so a chain of n small
    // elements carries n fewer pointers of overhead. What it costs is on display
    // above -- every edit needs the node BEFORE the one you care about.
    return 0;
}
