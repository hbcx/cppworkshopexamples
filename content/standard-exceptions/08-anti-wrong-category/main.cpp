// Anti-pattern: throwing the wrong family. The split is by CAUSE:
// logic_error = a bug the caller could have prevented (fix the code);
// runtime_error = an external condition to handle (retry, report, fall back).
// Labeling one as the other misleads every handler that reacts by family -- a
// real bug gets retried forever, or a transient failure gets logged as a bug.
#include <iostream>
#include <stdexcept>
#include <string>

// WRONG: a caller passing a bad index is a bug, but this reports it as a runtime
// condition, so a retry policy would treat a guaranteed failure as "try again".
int at_wrong(const std::string& s, int i) {
    if (i < 0 || i >= static_cast<int>(s.size())) {
        throw std::runtime_error("bad index");                 // mislabeled
    }
    return s[static_cast<std::size_t>(i)];
}

// RIGHT: an invalid index is a precondition violation -> out_of_range, which is
// a logic_error. (Bad input DATA from outside would be the runtime case.)
int at_right(const std::string& s, int i) {
    if (i < 0 || i >= static_cast<int>(s.size())) {
        throw std::out_of_range("index " + std::to_string(i) + " out of range");
    }
    return s[static_cast<std::size_t>(i)];
}

// A generic policy: fail fast on logic bugs, retry runtime failures.
void call(const char* label, int (*fn)(const std::string&, int)) {
    try {
        (void)fn("abc", 9);
        std::cout << label << ": ok\n";
    }
    catch (const std::logic_error& e) {
        std::cout << label << ": bug, fail fast     -- " << e.what() << '\n';
    }
    catch (const std::runtime_error& e) {
        std::cout << label << ": transient, retry   -- " << e.what() << '\n';
    }
}

int main() {
    call("wrong", at_wrong);   // a bug, but treated as retryable
    call("right", at_right);   // correctly a logic bug
    return 0;
}
