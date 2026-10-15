// B09 · LC 20: are the brackets matched and properly nested? Main: '(' ')' only.
// Follow-up: '(' ')' '[' ']' '{' '}'. Any other char is ignored.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Depth counter: never below 0, and 0 at the end. T: O(n), S: O(1).
bool isValid(const string& s) {
    int depth = 0;   // '(' not yet closed
    for (char c : s) {
        if (c == '(') {
            depth++;
        } else if (c == ')') {
            // ')' with nothing open, e.g. ")("
            if (depth == 0) { return false; }
            depth--;
        }
    }
    return depth == 0;
}

// follow-up: push the closer each opener needs; a closer must match the top.
// T: O(n), S: O(n).
bool isValid2(const string& s) {
    vector<char> stk;   // closer each unclosed opener needs, innermost at back
    for (char c : s) {
        if (c == '(') {
            stk.push_back(')');
        } else if (c == '[') {
            stk.push_back(']');
        } else if (c == '{') {
            stk.push_back('}');
        } else if (c == ')' || c == ']' || c == '}') {
            if (stk.empty() || stk.back() != c) { return false; }
            stk.pop_back();
        }
    }
    return stk.empty();
}

void test(const string& label, const string& s, bool want) {
    cout << label << (isValid(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("nested", "(a(b)c)", true);
    test("dips below 0", "())(", false);
    test("unclosed", "(()", false);
}
