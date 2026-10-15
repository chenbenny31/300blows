// LC 14: return the longest common prefix of an array of strings ("" if none).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Vertical scan: compare column i across all strings. T: O(total chars), S: O(1).
string longestCommonPrefix(const vector<string>& strs) {
    if (strs.empty()) { return ""; }
    int len = strs[0].size();
    for (int i = 0; i < len; i++) {
        char c = strs[0][i];
        for (const auto& s : strs) {
            // a shorter string or a different char ends the prefix at i
            if (i == (int)s.size() || s[i] != c) { return strs[0].substr(0, i); }
        }
    }
    return strs[0];
}

void test(const string& label, const vector<string>& strs, const string& want) {
    cout << label << (longestCommonPrefix(strs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {"flower", "flow", "flight"}, "fl");
    test("no prefix", {"dog", "racecar", "car"}, "");
    test("whole word", {"ab", "abc"}, "ab");
}
