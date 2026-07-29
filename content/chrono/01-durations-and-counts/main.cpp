#include <chrono>
#include <iostream>

int main() {
    std::chrono::seconds      threeSec(3);
    std::chrono::milliseconds halfSec(500);

    std::cout << "threeSec.count() = " << threeSec.count() << " (unit: seconds)\n";
    std::cout << "halfSec.count()  = " << halfSec.count() << " (unit: milliseconds)\n";

    // Same count, different meaning: the unit is part of the type, not a comment.
    std::chrono::seconds      sec3(3);
    std::chrono::milliseconds ms3(3);
    std::cout << "sec3 and ms3 both count 3, but sec3 == ms3? "
              << (sec3 == ms3 ? "yes" : "no") << '\n';   // no: 3s != 3ms

    // The count type (Rep) is just an integer under the hood.
    std::chrono::minutes twoMin(2);
    std::cout << "twoMin.count() = " << twoMin.count() << " (unit: minutes)\n";
    return 0;
}
