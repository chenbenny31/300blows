// B20 / LC 344 prototype: reverse each maximal run of ASCII letters/digits in place; separators stay where they are.

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

bool isAlnum(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

class RunReverser {
public:
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

    // build a copy, flush the buffered run reversed at each separator, T: O(n), S: O(n)
    string reverseRuns2(const string& s) {
        string out;
        string run;   // current alphanumeric run, not yet written
        for (char c : s) {
            if (isAlnum(c)) {
                run += c;
                continue;
            }
            out.append(run.rbegin(), run.rend());
            run.clear();
            out += c;
        }
        out.append(run.rbegin(), run.rend());   // last run has no separator after it
        return out;
    }
};

bool test(const string& s, const string& expected) {
    RunReverser rr;
    string got = s;
    rr.reverseRuns(got);
    string got2 = rr.reverseRuns2(s);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL \"" << s << "\": got \"" << got << "\" / \"" << got2 << "\", expected \"" << expected << "\"\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("ab12-cd!", "21ba-dc!");                  // given example
    ok &= test("", "");                                  // empty string
    ok &= test("a", "a");                                // single char run
    ok &= test("abc", "cba");                            // no separator, last run flushed
    ok &= test("---", "---");                            // only separators
    ok &= test("!!ab..c", "!!ba..c");                    // leading and doubled separators
    ok &= test("Hello, World 42", "olleH, dlroW 24");    // mixed case, space separators
    ok &= test("@AZ[`az{/09:", "@ZA[`za{/90:");          // chars just outside each range are separators
    ok &= test("ab\xC3\xA9" "cd", "ba\xC3\xA9" "dc");    // UTF-8 bytes (negative char) are separators
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
