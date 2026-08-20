#include <iostream>
#include <map>
#include <string>
#include <string_view>

int main() {
    // std::less<> (the transparent comparator) lets find() accept any type that
    // is comparable with the key. With a std::string key that includes a
    // std::string_view, so the lookup builds NO temporary std::string key.
    std::map<std::string, int, std::less<>> ages = {
        {"ada", 36}, {"alan", 41}, {"grace", 45}
    };

    std::string_view who = "alan";

    auto it = ages.find(who);          // no std::string constructed from `who`
    if (it != ages.end())
        std::cout << it->first << " -> " << it->second << '\n';

    // A literal works the same way -- also no temporary key.
    std::cout << std::boolalpha
              << "contains \"grace\": " << (ages.find("grace") != ages.end()) << '\n'
              << "contains \"eve\":   " << (ages.find("eve") != ages.end()) << '\n';

    // Note: a plain std::map<std::string,int> (default std::less<std::string>)
    // has NO transparent find, so find(who) there would first build a
    // std::string from `who` -- an allocation on every lookup.
    return 0;
}
