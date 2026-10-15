// LC 72: min number of insert / delete / replace operations to turn word1 into word2.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;

// 2D dp over prefixes. T: O(m*n), S: O(m*n).
int minDistance(const string& word1, const string& word2) {
    int m = word1.size(), n = word2.size();
    // dp[i][j] = edits to turn word1[0..i) into word2[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) { dp[i][0] = i; }   // delete all i chars
    for (int j = 0; j <= n; j++) { dp[0][j] = j; }   // insert all j chars
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // replace, delete from word1, insert into word1
                dp[i][j] = 1 + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& word1, const string& word2, int want) {
    cout << label << (minDistance(word1, word2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", "horse", "ros", 3);
    test("lc ex2", "intention", "execution", 5);
    test("empty", "", "abc", 3);
}
