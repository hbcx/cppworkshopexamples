#include <iostream>

// Mark the old API deprecated, with a message pointing at the replacement.
[[deprecated("use fastHash instead")]]
unsigned slowHash(const char* s) {
    unsigned h = 0;
    for (; *s; ++s)
        h = h * 31u + static_cast<unsigned>(*s);
    return h;
}

unsigned fastHash(const char* s) {   // the replacement
    unsigned h = 2166136261u;
    for (; *s; ++s) {
        h ^= static_cast<unsigned>(*s);
        h *= 16777619u;
    }
    return h;
}

int main() {
    std::cout << "fastHash(abc) = " << fastHash("abc") << "\n";

    // Old code can keep calling slowHash during migration. Each call emits:
    //   warning: 'slowHash' is deprecated: use fastHash instead
    // We silence that one warning locally so this file still builds under -Werror;
    // in real code the warning is exactly the nudge that drives the migration.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    std::cout << "slowHash(abc) = " << slowHash("abc") << "\n";
#pragma GCC diagnostic pop
    return 0;
}
