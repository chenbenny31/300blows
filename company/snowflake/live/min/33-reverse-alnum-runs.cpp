// B20 · LC 344 prototype: reverse each maximal run of ASCII letters/digits in place; separators stay where they are.
#include <iostream>
#include <string>
using std::cout;
using std::string;

// explicit ranges: std::isalnum is locale-dependent and UB on negative char
bool isAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

// find each run [i, j), two-pointer swap inside it, T: O(n), S: O(1)
void reverseRuns(string& s) {
    int n = s.size();
    int i = 0;
    while (i < n) {
        if (!isAlnum(s[i])) {
            i++;
            continue;
        }
        int j = i;
        while (j < n && isAlnum(s[j])) { j++; }
        for (int l = i, r = j - 1; l < r; l++, r--) { std::swap(s[l], s[r]); }
        i = j;
    }
}

void test(string s, const string& expect) {
    reverseRuns(s);
    cout << (s == expect ? "PASS " : "FAIL ") << s << " expect " << expect << "\n";
}

int main() {
    test("ab12-cd!", "21ba-dc!");                 // target example
    test("", "");                                 // empty string
    test("abc", "cba");                           // no separator, run ends at n
    test("!!ab..c", "!!ba..c");                   // leading and doubled separators
    test("@AZ[`az{/09:", "@ZA[`za{/90:");         // chars just outside each range are separators
}
