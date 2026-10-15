// ByteDance / LC 221: m x n matrix of '0' and '1'; return the area of the largest
// square containing only '1's.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// DP on the bottom-right corner of each square. T: O(R*C), S: O(R*C).
int maximalSquare(const vector<vector<char>>& matrix) {
    int rows = matrix.size(), cols = matrix[0].size();
    // dp[i][j] = side of the largest all-'1' square ending at cell (i-1, j-1)
    vector<vector<int>> dp(rows + 1, vector<int>(cols + 1, 0));
    int side = 0;
    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= cols; j++) {
            if (matrix[i - 1][j - 1] != '1') { continue; }
            // the smallest neighbor square limits how far this one extends
            dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            side = max(side, dp[i][j]);
        }
    }
    return side * side;
}

void test(const string& label, const vector<vector<char>>& matrix, int want) {
    cout << label << (maximalSquare(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{'1', '0', '1', '0', '0'},
                    {'1', '0', '1', '1', '1'},
                    {'1', '1', '1', '1', '1'},
                    {'1', '0', '0', '1', '0'}}, 4);
    test("diagonal", {{'0', '1'}, {'1', '0'}}, 1);
    test("no ones", {{'0'}}, 0);
}
