#include <numeric>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> parts{"aaa", "bbb", "ccc", "ddd", "eee"};   // 5 parts, 3 chars each

    // BAD: folding a string with acc + s copies the WHOLE growing accumulator at
    // every step, so the total work is O(n^2) in the characters, not O(n). We count
    // the characters each acc + p builds to make the cost visible.
    long copiedBad = 0;
    std::string bad = std::accumulate(parts.begin(), parts.end(), std::string{},
        [&copiedBad](const std::string& acc, const std::string& p){
            copiedBad += static_cast<long>(acc.size() + p.size());   // acc + p builds this many chars
            return acc + p;
        });
    std::cout << "accumulate with +: \"" << bad << "\", copied " << copiedBad << " chars\n";

    // GOOD: a plain loop with += appends each part onto one string, so each part's
    // characters are copied once -- O(n) total. reserve() up front avoids regrowth.
    long copiedGood = 0;
    std::string good;
    good.reserve(15);
    for (const std::string& p : parts) {
        copiedGood += static_cast<long>(p.size());   // only p's characters are appended
        good += p;
    }
    std::cout << "loop with +=:      \"" << good << "\", copied " << copiedGood << " chars\n";
    return 0;
}
