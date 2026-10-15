// LC 28 Find the Index of the First Occurrence in a String: first index where needle
// occurs in haystack, -1 if none; an empty needle matches at 0.
// Trick: KMP - on a mismatch fall back to lps[j-1], never moving back in the haystack
// Recognize: substring search in O(n + m), or any "longest prefix that is also a suffix"
// Follow-up LC 214: shortest palindrome by adding chars in front = lps of s#reverse(s).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// lps[i] = length of the longest proper prefix of p[0..i] that is also its suffix
vector<int> buildLps(const string& p) {
    int m = p.size();
    vector<int> lps(m, 0);
    int len = 0;   // length of the current matched prefix
    for (int i = 1; i < m; i++) {
        // shrink to the next shorter border until p[i] can extend it
        while (len > 0 && p[i] != p[len]) { len = lps[len - 1]; }
        if (p[i] == p[len]) { len++; }
        lps[i] = len;
    }
    return lps;
}

// KMP scan. T: O(n + m), S: O(m).
int strStr(const string& haystack, const string& needle) {
    int n = haystack.size(), m = needle.size();
    if (m == 0) { return 0; }
    vector<int> lps = buildLps(needle);
    int j = 0;   // chars of needle matched so far
    for (int i = 0; i < n; i++) {
        // the matched part's border is already known to match, so i never goes back
        while (j > 0 && haystack[i] != needle[j]) { j = lps[j - 1]; }
        if (haystack[i] == needle[j]) { j++; }
        if (j == m) { return i - m + 1; }
    }
    return -1;
}

// follow-up LC 214: the last lps value = longest palindromic prefix of s; '#' stops a
// border from running past s. Prepend the reversed rest. T: O(n), S: O(n).
string shortestPalindrome(const string& s) {
    string rev(s.rbegin(), s.rend());
    vector<int> lps = buildLps(s + "#" + rev);
    int keep = lps.back();   // length of the longest palindromic prefix of s
    return rev.substr(0, s.size() - keep) + s;
}

void test(const string& label, const string& haystack, const string& needle, int want) {
    cout << label << (strStr(haystack, needle) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "sadbutsad", "sad", 0);
    test("lc ex2", "leetcode", "leeto", -1);
    test("fallback", "aabaaabaaac", "aabaaac", 4);
    test("empty needle", "a", "", 0);
}
