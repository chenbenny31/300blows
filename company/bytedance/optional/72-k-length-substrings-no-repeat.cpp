// ByteDance (hack2hire) / LC 1100: count the substrings of s (lowercase) of length k
// that have no repeated characters.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Fixed window of size k, tracking how many letters repeat inside. T: O(n), S: O(1).
int numKLenSubstrNoRepeats(const string& s, int k) {
    // pigeonhole: more than 26 lowercase letters must repeat
    if (k > 26) { return 0; }
    vector<int> cnt(26, 0);   // cnt[c] = occurrences of letter c in the window
    int dup = 0;              // letters with cnt >= 2 in the window
    int result = 0;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        if (++cnt[s[i] - 'a'] == 2) { dup++; }
        if (i >= k && --cnt[s[i - k] - 'a'] == 1) { dup--; }
        if (i >= k - 1 && dup == 0) { result++; }
    }
    return result;
}

void test(const string& label, const string& s, int k, int want) {
    cout << label << (numKLenSubstrNoRepeats(s, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "havefunonleetcode", 5, 6);
    test("k > n", "home", 5, 0);
}
