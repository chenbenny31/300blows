// LC 338 Counting Bits: for every i in 0..n return the number of 1 bits in i,
// in O(n) total.
// Trick: dp[i] = dp[i >> 1] + (i & 1) - i is i/2 shifted left plus its lowest bit
// Recognize: popcount for a whole range -> reuse the answer of a smaller number
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// i >> 1 < i is already computed; the shift drops only the lowest bit. T: O(n), S: O(n).
vector<int> countBits(int n) {
    vector<int> dp(n + 1, 0);   // dp[i] = number of 1 bits in i
    for (int i = 1; i <= n; i++) { dp[i] = dp[i >> 1] + (i & 1); }
    return dp;
}

void test(const string& label, int n, const vector<int>& want) {
    cout << label << (countBits(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", 2, {0, 1, 1});
    test("lc ex2", 5, {0, 1, 1, 2, 1, 2});
}
