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
    std::forward_list<std::string> ready{"a", "b", "c"};
    std::forward_list<std::string> done{"x", "y"};

    // Move ONE element: the node after ready.begin() ("b") is relinked to sit
    // after done.before_begin() (the head). No copy, no move of the string.
    done.splice_after(done.before_begin(), ready, ready.begin());
    print(ready, "ready after single splice: ");
    print(done,  "done after single splice:  ");

    // Move a RANGE (open interval): everything strictly between before_begin and
    // end of `ready` -- here the remaining "a" and "c" -- appended after "b".
    done.splice_after(done.begin(), ready,
                      ready.before_begin(), ready.end());
    print(ready, "ready after range splice: ");
    print(done,  "done after range splice:  ");

    // Move a WHOLE list: relink all of `extra` after done's head. extra ends empty.
    std::forward_list<std::string> extra{"m", "n"};
    done.splice_after(done.before_begin(), extra);
    print(done,  "done after whole-list splice: ");
    std::cout << "extra empty? " << std::boolalpha << extra.empty() << '\n';
    return 0;
}
