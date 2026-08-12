#include <ranges>
#include <vector>
#include <string>
#include <iostream>

// zip's length is the MINIMUM of its inputs. When the ranges have different
// lengths the tail of the longer one is silently dropped -- no error, no
// warning. If the lengths were supposed to match, this hides a bug.

int main() {
    std::vector<std::string> ids{"a", "b", "c", "d"};
    std::vector<int> values{10, 20, 30};   // one short -- a data bug upstream

    // BAD: we believe we process all four ids, but zip stops after three.
    int processed = 0;
    for (auto [id, val] : std::views::zip(ids, values)) {
        (void)id; (void)val;
        ++processed;
    }
    std::cout << "ids: " << ids.size() << ", processed: " << processed
              << "  <- 'd' silently skipped\n";

    // FIX: when the ranges MUST be the same length, check first so a mismatch is
    // a loud error instead of quietly lost data.
    if (ids.size() != values.size())
        std::cout << "size mismatch detected: " << ids.size()
                  << " vs " << values.size() << " -- handle it, do not zip blindly\n";
    return 0;
}
