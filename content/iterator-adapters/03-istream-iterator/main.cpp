#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int main() {
    // istream_iterator<T> reads a T from a stream on each ++. A DEFAULT-constructed
    // istream_iterator is the end sentinel, so [begin, end) runs until the stream
    // stops producing Ts. (An istringstream keeps this example self-contained.)
    std::istringstream in("10 20 30 40 50");
    std::istream_iterator<int> begin(in);
    std::istream_iterator<int> end;   // end-of-stream marker

    std::vector<int> nums(begin, end);
    std::cout << "read " << nums.size() << " ints, sum = "
              << std::accumulate(nums.begin(), nums.end(), 0) << "\n";   // 5, 150

    // The same idea for words: copy a whitespace-separated stream into a vector.
    std::istringstream words("alpha beta gamma");
    std::istream_iterator<std::string> wbegin(words), wend;
    std::vector<std::string> ws(wbegin, wend);
    std::cout << "words:";
    for (const auto& w : ws) std::cout << " " << w;
    std::cout << "\n";   // alpha beta gamma
    return 0;
}
