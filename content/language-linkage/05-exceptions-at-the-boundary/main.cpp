#include <iostream>
#include <stdexcept>

// A function that C code might call must NOT let a C++ exception escape -- unwinding
// through a C caller's frame is undefined. So an extern "C" function catches at the
// boundary and reports failure the way C expects: a return code plus an out-param.
extern "C" int safe_divide(int a, int b, int* out) {
    try {
        if (b == 0)
            throw std::runtime_error("division by zero");
        *out = a / b;
        return 0;                       // success
    } catch (const std::exception& e) {
        std::cout << "  (caught at the boundary: " << e.what() << ")\n";
        return 1;                       // failure -- no exception leaves this function
    }
}

int main() {
    int result = 0;
    if (safe_divide(10, 2, &result) == 0)
        std::cout << "10 / 2 = " << result << "\n";

    if (safe_divide(10, 0, &result) != 0)
        std::cout << "10 / 0 reported an error, no exception escaped\n";
    return 0;
}
