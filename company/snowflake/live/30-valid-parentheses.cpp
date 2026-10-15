// B09 / LC 20: check brackets are matched and nested; base only '(' ')', follow-up adds '[' ']' '{' '}'. Other chars ignored.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;

class BracketChecker {
public:
    // depth counter, T: O(n), S: O(1)
    bool isValid(const string& s) {
        int depth = 0;   // currently unclosed '('
        for (char c : s) {
            if (c == '(') {
                depth++;
            } else if (c == ')') {
                if (depth == 0) { return false; }   // close before any open, e.g. ")("
                depth--;
            }
        }
        return depth == 0;
    }

    // stack of expected closers, T: O(n), S: O(n)
    bool isValid2(const string& s) {
        vector<char> expect;   // closer each unclosed opener needs, innermost at back
        for (char c : s) {
            if (c == '(') {
                expect.push_back(')');
            } else if (c == '[') {
                expect.push_back(']');
            } else if (c == '{') {
                expect.push_back('}');
            } else if (c == ')' || c == ']' || c == '}') {
                if (expect.empty() || expect.back() != c) { return false; }   // check empty before back()
                expect.pop_back();
            }
        }
        return expect.empty();
    }
};

bool testParen(const string& s, bool expected) {
    BracketChecker bc;
    bool got = bc.isValid(s);
    bool got2 = bc.isValid2(s);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL \"" << s << "\": got " << got << " / " << got2 << ", expected " << expected << "\n";
    return false;
}

bool testAll(const string& s, bool expected) {
    BracketChecker bc;
    bool got = bc.isValid2(s);
    if (got == expected) { return true; }
    cout << "FAIL all-kinds \"" << s << "\": got " << got << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= testParen("()", true);          // smallest valid pair
    ok &= testParen("(()())", true);      // nested and sequential
    ok &= testParen("", true);            // empty string
    ok &= testParen("(()", false);        // unclosed at end
    ok &= testParen(")(", false);         // close before open
    ok &= testParen("())(", false);       // net zero but dips negative
    ok &= testParen("a(b)c", true);       // non-bracket chars ignored
    ok &= testParen("((((((", false);     // only openers
    ok &= testAll("()[]{}", true);        // LC example 2
    ok &= testAll("(]", false);           // LC example 3: wrong kind
    ok &= testAll("{[]}", true);          // nested mixed kinds
    ok &= testAll("([)]", false);         // per-kind counters would say true
    ok &= testAll("]", false);            // closer on empty stack
    ok &= testAll("[", false);            // opener left on stack
    ok &= testAll("x[y{z}w]v", true);     // non-bracket chars ignored
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
