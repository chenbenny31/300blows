// reverse every maximal run of ASCII letters/digits; every other character stays where it is (Q43, J10690)
// "ab12-cd!" -> "21ba-dc!"

// alphanumeric = ASCII A-Z a-z 0-9 only? (std::isalnum depends on locale)
// modify in place, or return a new string?
// empty string, no separators, only separators?

#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::string;

class RunReverser {
public:
    // 1: build a new string, flush each run reversed when a separator shows up, T: O(n), S: O(n)
    string reverseRuns_1(const string& s) {
        string out;
        string run; // current run of alphanumerics, not yet written
        for (char c : s) {
            if (isAlnum(c)) {
                run += c;
            } else {
                out.append(run.rbegin(), run.rend());
                run.clear();
                out += c; // separator keeps its position
            }
        }
        out.append(run.rbegin(), run.rend()); // last run has no separator after it
        return out;
    }

    // 2: in place, find each run [i, j) and reverse it, T: O(n), S: O(1) extra
    void reverseRuns_2(string& s) {
        int n = static_cast<int>(s.size());
        int i = 0;
        while (i < n) {
            if (!isAlnum(s[i])) {
                i++;
                continue;
            }
            int j = i;
            while (j < n && isAlnum(s[j])) {
                j++;
            }
            std::reverse(s.begin() + i, s.begin() + j); // two-pointer swap inside [i, j)
            i = j;
        }
    }

private:
    bool isAlnum(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
    }
};

int main() {
    RunReverser rr;
    string s;

    assert(rr.reverseRuns_1("ab12-cd!") == "21ba-dc!"); // target
    s = "ab12-cd!";
    rr.reverseRuns_2(s);
    assert(s == "21ba-dc!");

    assert(rr.reverseRuns_1("!!ab..c") == "!!ba..c"); // edge: separators at the start, back to back
    s = "!!ab..c";
    rr.reverseRuns_2(s);
    assert(s == "!!ba..c");

    assert(rr.reverseRuns_1("abc") == "cba"); // edge: no separator, last run flushed
    s = "abc";
    rr.reverseRuns_2(s);
    assert(s == "cba");

    assert(rr.reverseRuns_1("") == ""); // null
    s = "";
    rr.reverseRuns_2(s);
    assert(s == "");

    std::cout << "all tests passed\n";
    return 0;
}
