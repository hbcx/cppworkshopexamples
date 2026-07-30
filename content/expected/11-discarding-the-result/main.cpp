#include <expected>
#include <iostream>
#include <string>

static bool committed = false;

// Returns expected<void, E>: succeeds, or fails with a reason.
std::expected<void, std::string> commit(int amount) {
    if (amount <= 0) return std::unexpected("amount must be positive");
    committed = true;
    return {};
}

int main() {
    std::cout << std::boolalpha;

    // Anti-pattern: calling something that returns expected and ignoring the
    // result. The error is right there in the return value -- and thrown away.
    // Control falls through as if it had succeeded.
    // (libstdc++ marks std::expected [[nodiscard]], so this bare call actually
    //  warns -- this example disables -Werror to let the drop through and show
    //  its effect. Treat that warning as the error it is.)
    committed = false;
    commit(-5);   // fails, but nobody looks; the error vanishes
    std::cout << "[buggy]   ignored commit(-5): committed=" << committed
              << " -- yet the code moved on as if it worked\n";

    // Fix: always inspect the result. expected exists precisely so the failure
    // is a value you must handle, not a hidden throw -- do not drop it.
    committed = false;
    if (auto r = commit(-5); !r) {
        std::cout << "[correct] commit(-5) failed: " << r.error() << '\n';
    } else {
        std::cout << "[correct] commit(-5) ok\n";
    }
    return 0;
}
