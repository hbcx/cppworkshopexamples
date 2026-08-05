// Open-Closed at compile time -- policy-based design.
//
// The Serializer is fixed; the FORMAT is a template policy. A new output format
// is a new policy type, resolved at compile time with no virtual calls and no
// edit to Serializer -- open for extension, closed for modification, statically.

#include <iostream>
#include <string>

// Each policy is a small type providing the same static operations.
struct JsonFormat {
    static std::string open() { return "{"; }
    static std::string close() { return "}"; }
    static std::string pair(const std::string& k, const std::string& v) {
        return "\"" + k + "\":\"" + v + "\",";
    }
};

struct XmlFormat {
    static std::string open() { return "<record>"; }
    static std::string close() { return "</record>"; }
    static std::string pair(const std::string& k, const std::string& v) {
        return "<" + k + ">" + v + "</" + k + ">";
    }
};

// Written once, against the policy -- never edited to add a format.
template <class Format>
class Serializer {
public:
    std::string serialize(const std::string& name, const std::string& city) const {
        return Format::open() + Format::pair("name", name) +
               Format::pair("city", city) + Format::close();
    }
};

// EXTENSION: a new format added later -- Serializer is not touched.
struct CsvFormat {
    static std::string open() { return ""; }
    static std::string close() { return ""; }
    static std::string pair(const std::string&, const std::string& v) {
        return v + ";";
    }
};

int main() {
    Serializer<JsonFormat> json;
    Serializer<XmlFormat> xml;
    Serializer<CsvFormat> csv; // the new policy, no change to Serializer

    std::cout << "json: " << json.serialize("Ada", "London") << "\n";
    std::cout << "xml:  " << xml.serialize("Ada", "London") << "\n";
    std::cout << "csv:  " << csv.serialize("Ada", "London") << "\n";
}
