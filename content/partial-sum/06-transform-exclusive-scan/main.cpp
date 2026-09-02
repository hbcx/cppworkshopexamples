#include <numeric>
#include <iterator>
#include <vector>
#include <string>
#include <functional>
#include <cstddef>
#include <iostream>

struct Record { std::string name; int payload; };

int main() {
    // Each record serializes to (4-byte length prefix + payload) bytes on the
    // wire. We want the byte OFFSET where each record starts -- the total size
    // of everything before it. transform_exclusive_scan maps each record to its
    // on-wire size and exclusive-scans, so offset[0] is the init and offset[i]
    // excludes record i itself.
    std::vector<Record> records{
        {"alpha", 10}, {"beta", 20}, {"gamma", 5}, {"delta", 8}};

    auto wireSize = [](const Record& r) { return 4 + r.payload; };

    std::vector<int> offsets;
    // The exclusive form flips the argument order versus the inclusive one:
    // init comes FIRST (right after the output), THEN the binary combine, THEN
    // the unary transform. Mixing the two orders up is the classic mistake.
    std::transform_exclusive_scan(
        records.begin(), records.end(), std::back_inserter(offsets),
        0,                 // init: the first record starts at byte 0
        std::plus<>{},     // binary: combine sizes
        wireSize);         // unary: size of one record

    for (std::size_t i = 0; i < records.size(); ++i)
        std::cout << "record " << records[i].name
                  << " starts at byte " << offsets[i] << '\n';
    return 0;
}
