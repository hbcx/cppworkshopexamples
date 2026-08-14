#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    std::vector<int> source{20, -1, 22, -1, 25};

    // std::replace_copy writes the range to a NEW destination, substituting as it
    // goes, and leaves the source untouched. Use a back_inserter so the output
    // grows to fit (the destination need not be pre-sized this way).
    std::vector<int> cleaned;
    std::replace_copy(source.begin(), source.end(),
                      std::back_inserter(cleaned), -1, 0);

    print("source (unchanged):", source);
    print("cleaned copy:      ", cleaned);

    // The output is always the SAME LENGTH as the input -- one element in, one
    // element out, some of them substituted. This is the non-mutating form: reach
    // for it when you must keep the original, e.g. to produce a normalized view
    // for display while the raw data stays intact.
    return 0;
}
