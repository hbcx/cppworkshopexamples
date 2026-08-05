// Anti-pattern: extending behaviour with boolean flag parameters.
//
// Exporter::run grew a bool for each new step -- upper-case, add a header, sort.
// Every new behaviour adds a parameter and a branch inside the class, which
// swells without bound and reopens tested code each time. And run(data, true,
// false, true) is unreadable at the call site.
//
// The fix makes each step an object and runs a pipeline of them. A new step is
// a new object appended to the list; Exporter is closed.

#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// ---------- a flag (and a branch) per behaviour ----------
namespace bad {

class Exporter {
public:
    std::string run(std::vector<std::string> rows,
                    bool upper, bool header, bool sortRows) const {
        if (sortRows) std::sort(rows.begin(), rows.end());
        std::string out;
        if (header) out += "== export ==\n";
        for (std::string r : rows) {
            if (upper)
                for (char& c : r)
                    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            out += r + "\n";
        }
        return out; // adding "dedupe" means a new bool and a new branch in here
    }
};

} // namespace bad

// ---------- each behaviour is a composable step ----------
namespace good {

using Step = std::function<void(std::vector<std::string>&)>;

class Exporter {
public:
    void addStep(Step s) { steps_.push_back(std::move(s)); }

    std::string run(std::vector<std::string> rows) const {
        for (const Step& s : steps_) s(rows);
        std::string out;
        for (const std::string& r : rows) out += r + "\n";
        return out;
    }

private:
    std::vector<Step> steps_;
};

} // namespace good

int main() {
    std::vector<std::string> data;
    data.push_back("banana");
    data.push_back("apple");
    data.push_back("cherry");

    bad::Exporter legacy;
    std::cout << "[bad]\n" << legacy.run(data, true, false, true);

    good::Exporter exporter;
    exporter.addStep([](std::vector<std::string>& rows) { std::sort(rows.begin(), rows.end()); });
    exporter.addStep([](std::vector<std::string>& rows) {
        for (std::string& r : rows)
            for (char& c : r)
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    });
    // EXTENSION: a new step is a new object, not a new flag inside Exporter.
    exporter.addStep([](std::vector<std::string>& rows) {
        for (std::string& r : rows) r = "- " + r;
    });
    std::cout << "[good]\n" << exporter.run(data);
}
