// ByteDance (hack2hire) / LC 10: does pattern p match the whole string s? '.' matches
// any single char; '*' matches zero or more of the element before it.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Bottom-up dp over prefixes. T: O(m*n), S: O(m*n).
bool isMatch(const string& s, const string& p) {
    int m = s.size(), n = p.size();
    // dp[i][j] = s[0..i) matches p[0..j)
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;
    for (int i = 0; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == '*') {
                // zero copies: drop "x*"
                bool zero = dp[i][j - 2];
                // one more copy: s[i-1] matches x, and s[0..i-1) still matches "..x*"
                bool more = i > 0 && (p[j - 2] == '.' || p[j - 2] == s[i - 1])
                            && dp[i - 1][j];
                dp[i][j] = zero || more;
            } else {
                dp[i][j] = i > 0 && (p[j - 1] == '.' || p[j - 1] == s[i - 1])
                           && dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& s, const string& p, bool want) {
    cout << label << (isMatch(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("too short", "aa", "a", false);
    test("star repeat", "aa", "a*", true);
    test("dot star", "ab", ".*", true);
    test("star zero", "aab", "c*a*b", true);
}
