// B09 · LC 20: brackets matched and nested; base only '(' ')', other chars ignored. Follow-up: also '[' ']' '{' '}'.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

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

// follow-up: stack of expected closers, T: O(n), S: O(n)
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

void test(const string& s, bool expect) {
    bool got = isValid(s);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

void testAll(const string& s, bool expect) {
    bool got = isValid2(s);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    test("(()())", true);     // nested and sequential
    test("", true);           // empty string
    test("(()", false);       // unclosed at end
    test("())(", false);      // net zero but dips negative
    test("a(b)c", true);      // other chars ignored

    testAll("()[]{}", true);  // LC example 2
    testAll("([)]", false);   // per-kind counters would say true
    testAll("]", false);      // closer on empty stack
    testAll("[", false);      // opener left on stack
}
