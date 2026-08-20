#include <iostream>
#include <regex>
#include <string>

int main() {
    std::cout << std::boolalpha;

    // WRONG (described, NOT run): nested quantifiers over overlapping matches let
    // the engine try exponentially many ways to split the input before it can
    // report failure. On N letters 'a' followed by a non-'a', the pattern
    //   ^(a+)+$
    // takes on the order of 2^N steps -- a few dozen characters can hang for
    // seconds or minutes. On attacker-controlled input this is a denial of
    // service (ReDoS). We deliberately do not run it.
    //   std::regex bad(R"(^(a+)+$)");
    //   std::regex_match(std::string(40, 'a') + "!", bad);   // would blow up

    // RIGHT: the same language without the nesting. ^a+$ matches one-or-more 'a'
    // in linear time -- there is no second quantifier to backtrack through.
    std::regex safe(R"(^a+$)");
    std::string ok(40, 'a');
    std::string no = ok + "!";
    std::cout << "safe on 40 a's       : " << std::regex_match(ok, safe) << '\n';  // true, instant
    std::cout << "safe on 40 a's + '!' : " << std::regex_match(no, safe) << '\n';  // false, instant

    // The rule: avoid a quantifier applied to a group that itself repeats a
    // quantified atom -- (a+)+, (a*)*, (.*)* -- because the same input can be
    // divided many ways. Rewrite to one quantifier, or constrain the input.
    return 0;
}
