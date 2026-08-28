#include <iostream>

// Anti-pattern: casting a raw number into an enum without checking it names a
// real enumerator. static_cast<Enum>(n) is allowed for any value in the
// underlying type's range, so it happily produces an enum value that matches
// none of your cases -- and a switch then silently falls through to nothing.

enum class Status : int { Ok = 0, Warning = 1, Error = 2 };

const char* describe(Status s) {
    switch (s) {
        case Status::Ok:      return "ok";
        case Status::Warning: return "warning";
        case Status::Error:   return "error";
    }
    return "<unknown status>";   // reached for any value that is not listed
}

int main() {
    // A value that came from a file, socket or cast -- 99 is not an enumerator:
    Status bad = static_cast<Status>(99);

    // Well defined (99 fits the underlying int), but no case matches:
    std::cout << "status 99 -> " << describe(bad) << "\n";           // <unknown status>
    std::cout << "as number  -> " << static_cast<int>(bad) << "\n";  // 99

    // THE FIX: validate before trusting a cast -- check the number is one of the
    // known enumerators (a range check or a lookup) before making it a Status,
    // and always handle the "unknown" path.
    int raw = 99;
    bool valid = (raw >= 0 && raw <= 2);
    std::cout << "is 99 a valid Status? " << valid << "\n";          // 0
    return 0;
}
