// ByteDance (hack2hire): repeatedly remove every contiguous triple "xXx" (a lowercase
// letter, any uppercase letter, the same lowercase letter, e.g. "aAa", "vQv") until
// none is left; return the final string.
#include <iostream>
#include <string>
using std::cout;
using std::string;

bool isLower(char c) { return c >= 'a' && c <= 'z'; }
bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }

// String as a stack; a removal can only complete at the top. T: O(n), S: O(n).
string filterCamel(const string& s) {
    string stk;   // chars kept so far, no removable triple inside
    for (char c : s) {
        stk.push_back(c);
        int m = stk.size();
        if (m >= 3 && isLower(stk[m - 1]) && isUpper(stk[m - 2])
            && stk[m - 3] == stk[m - 1]) {
            // popping may expose a new triple ending at the next pushed char
            stk.resize(m - 3);
        }
    }
    return stk;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (filterCamel(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("XxX kept", "AaADbDEeEbcvQv", "AaADbDEeEbc");
    test("one triple", "abBbc", "ac");
    test("cascade", "abBbAa", "");
}
