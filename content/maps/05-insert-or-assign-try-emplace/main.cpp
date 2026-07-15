#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, std::string> config;
    config["theme"] = "dark";

    // insert_or_assign: upsert -- overwrites if present, inserts if not, and tells
    // you which happened via the returned bool.
    auto a = config.insert_or_assign("theme", "light");   // key exists -> overwrite
    auto b = config.insert_or_assign("lang", "en");        // key absent -> insert
    std::cout << "theme now " << config["theme"]
              << " (inserted=" << (a.second ? "yes" : "no") << ")\n";
    std::cout << "lang  now " << config["lang"]
              << " (inserted=" << (b.second ? "yes" : "no") << ")\n";

    // try_emplace: insert only if absent, and do NOT touch the arguments when the
    // key already exists.
    auto c = config.try_emplace("theme", "solarized");     // key exists -> no change
    std::cout << "try_emplace theme: inserted=" << (c.second ? "yes" : "no")
              << " value=" << c.first->second << '\n';       // still light

    auto d = config.try_emplace("font", "mono");           // key absent -> inserts
    std::cout << "try_emplace font:  inserted=" << (d.second ? "yes" : "no")
              << " value=" << d.first->second << '\n';
    return 0;
}
