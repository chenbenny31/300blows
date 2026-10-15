// LC 1143: length of the longest common subsequence of text1 and text2.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// 2D dp over prefixes. T: O(m*n), S: O(m*n).
int longestCommonSubsequence(const string& text1, const string& text2) {
    int m = text1.size(), n = text2.size();
    // dp[i][j] = LCS length of text1[0..i) and text2[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // drop the last char of one string or the other
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& text1, const string& text2, int want) {
    bool ok = longestCommonSubsequence(text1, text2) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "abcde", "ace", 3);
    test("same", "abc", "abc", 3);
    test("disjoint", "abc", "def", 0);
}
