// Because an exception_ptr is a value, you can COLLECT several. A batch job that
// must attempt every item (not stop at the first failure) captures each error as
// an exception_ptr, finishes the batch, then reports them all -- instead of
// aborting on the first throw.
#include <iostream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>

void process(const std::string& item) {
    if (item.empty()) throw std::invalid_argument("empty item");
    if (item == "bad") throw std::runtime_error("item 'bad' rejected");
    std::cout << "processed: " << item << '\n';
}

int main() {
    std::vector<std::string> batch = {"one", "", "two", "bad", "three"};
    std::vector<std::exception_ptr> failures;

    for (const std::string& item : batch) {
        try {
            process(item);
        }
        catch (...) {
            failures.push_back(std::current_exception());   // keep going
        }
    }

    std::cout << "failures: " << failures.size() << '\n';
    for (const std::exception_ptr& ep : failures) {
        try {
            std::rethrow_exception(ep);
        }
        catch (const std::exception& e) {
            std::cout << "  - " << e.what() << '\n';
        }
    }
    return 0;
}
