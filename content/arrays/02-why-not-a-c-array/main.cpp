#include <array>
#include <iostream>

// The classic decay bug. Inside here, 'values' is an int*, NOT an int[5]: the
// size is gone and sizeof measures the pointer. This compiles clean.
static void count_c_array(const int values[5]) {
    std::cout << "  inside the function, sizeof(values)/sizeof(values[0]) = "
              << sizeof(values) / sizeof(values[0])
              << "   <- wrong: it measured the pointer\n";
}

// std::array does not decay. The size travels with the value.
static void count_std_array(const std::array<int, 5>& values) {
    std::cout << "  inside the function, values.size() = " << values.size()
              << "   <- right, and it cannot be lost\n";
}

int main() {
    int cArray[5] = {1, 2, 3, 4, 5};
    std::array<int, 5> stdArray{1, 2, 3, 4, 5};

    // 1. Size. Correct in the caller, meaningless one call later.
    std::cout << "in main, sizeof(cArray)/sizeof(cArray[0]) = "
              << sizeof(cArray) / sizeof(cArray[0]) << '\n';
    count_c_array(cArray);
    count_std_array(stdArray);

    // 2. Copying. A C array cannot be assigned:
    //    int other[5];
    //    other = cArray;          // does not compile
    // std::array is a value like any other:
    std::array<int, 5> copy = stdArray;
    copy[0] = 99;
    std::cout << "copy is independent: copy[0]=" << copy[0]
              << " original[0]=" << stdArray[0] << '\n';

    // 3. Comparison. This is the one that hurts: for C arrays == compares the
    // two ADDRESSES, so it is false even when every element matches -- and it
    // compiles without a word.
    int sameValues[5] = {1, 2, 3, 4, 5};
    const bool cArraysEqual = (cArray == sameValues);   // address comparison!
    std::cout << "C arrays with identical elements, cArray == sameValues: "
              << (cArraysEqual ? "true" : "false") << "   <- compared addresses\n";

    std::array<int, 5> sameStdValues{1, 2, 3, 4, 5};
    std::cout << "std::arrays with identical elements, a == b:            "
              << (stdArray == sameStdValues ? "true" : "false")
              << "    <- compared elements\n";
    return 0;
}
