#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Record {
    std::string id;
    int amount;
};

int main() {
    std::vector<Record> batch{
        {"A-1", 100},
        {"A-2", 250},
        {"A-3", 90},
    };
    int limit = 1000;   // a runtime threshold the predicate captures

    // A validation summary reads directly as three questions about the batch.
    bool allHaveId = std::all_of(batch.begin(), batch.end(),
        [](const Record& r){ return !r.id.empty(); });
    bool anyOverLimit = std::any_of(batch.begin(), batch.end(),
        [limit](const Record& r){ return r.amount > limit; });
    bool noneNegative = std::none_of(batch.begin(), batch.end(),
        [](const Record& r){ return r.amount < 0; });

    std::cout << std::boolalpha;
    std::cout << "all have an id?     " << allHaveId    << '\n';   // true
    std::cout << "any over the limit? " << anyOverLimit << '\n';   // false
    std::cout << "none negative?      " << noneNegative << '\n';   // true

    bool accepted = allHaveId && !anyOverLimit && noneNegative;
    std::cout << "batch accepted:     " << accepted << '\n';       // true
    return 0;
}
