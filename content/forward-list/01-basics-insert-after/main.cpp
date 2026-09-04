#include <forward_list>
#include <iostream>
#include <string>

static void print(const std::forward_list<std::string>& fl, const char* label) {
    std::cout << label;
    for (const std::string& s : fl)
        std::cout << ' ' << s;
    std::cout << '\n';
}

int main() {
    std::forward_list<std::string> chain{"beta", "gamma"};

    // Only the head is cheap: push_front is O(1). There is no push_back, because
    // reaching the tail would mean walking every node.
    chain.push_front("alpha");
    print(chain, "chain: ");

    // Nothing points backward, so you cannot insert BEFORE a node -- every
    // mutating call is named *_after and takes the node in front of it.
    auto first = chain.begin();                     // -> alpha
    chain.insert_after(first, "alpha-and-a-half");  // goes AFTER alpha
    print(chain, "after insert_after(begin): ");

    // erase_after removes the node FOLLOWING the given one, for the same reason.
    chain.erase_after(first);                        // drop alpha-and-a-half
    print(chain, "after erase_after(begin): ");

    // front() is O(1); back() does not exist.
    std::cout << "front = " << chain.front() << '\n';
    return 0;
}
