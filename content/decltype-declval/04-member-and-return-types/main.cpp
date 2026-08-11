#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

struct Widget {
    int id;
    std::vector<double> samples;
    std::string name() const { return "widget"; }
    double average() const { return 0.0; }
};

int main() {
    // Type of a data MEMBER: decltype(T::member) is its declared type.
    using IdType      = decltype(Widget::id);        // int
    using SamplesType = decltype(Widget::samples);   // std::vector<double>
    static_assert(std::is_same<IdType, int>::value, "id is int");
    static_assert(std::is_same<SamplesType, std::vector<double>>::value, "samples is vector<double>");

    // RETURN type of a member function: decltype over a declval-based call.
    using NameRet = decltype(std::declval<Widget>().name());      // std::string
    using AvgRet  = decltype(std::declval<Widget>().average());   // double
    static_assert(std::is_same<NameRet, std::string>::value, "name() returns std::string");
    static_assert(std::is_same<AvgRet, double>::value, "average() returns double");

    // Use the extracted type: this variable always matches name()'s return type,
    // even if name() is later changed to return something else.
    NameRet label = "computed";
    std::cout << "label=" << label << " (its type follows Widget::name()'s return type)\n";
    return 0;
}
