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
    std::forward_list<std::string> log{"open", "read"};

    // To touch the head you need a position BEFORE it. before_begin() is that
    // phantom position -- insert_after(before_begin, x) prepends, exactly what
    // push_front does.
    log.insert_after(log.before_begin(), "connect");
    print(log, "after insert_after(before_begin): ");

    // erase_after(before_begin()) drops the head -- the equivalent of pop_front.
    log.erase_after(log.before_begin());
    print(log, "after erase_after(before_begin): ");

    // Keep a trailing iterator (starting at before_begin) while walking, and you
    // always hold the node in FRONT of the current one. That is the position
    // erase_after needs to delete the current element by value -- here "read".
    auto prev = log.before_begin();
    for (auto it = log.begin(); it != log.end(); ++it) {
        if (*it == "read") {
            log.erase_after(prev);   // prev is the node before *it, so this drops "read"
            break;
        }
        prev = it;
    }
    print(log, "after deleting read by value: ");
    return 0;
}
