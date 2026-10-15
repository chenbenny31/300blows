// Apple (hack2hire): non-negative arr and target; does some subset (each element at most
// once) sum to exactly target? Use O(target) extra space, O(n * target) time.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// 0/1 knapsack in one row: n items x (target + 1) sums. T: O(n * target), S: O(target).
bool canSum(const vector<int>& arr, int target) {
    vector<bool> dp(target + 1, false);   // dp[s] = some subset of items so far sums to s
    dp[0] = true;
    for (int x : arr) {
        // high to low: dp[s - x] is still the row before x, so x is used at most once
        for (int s = target; s >= x; s--) {
            if (dp[s - x]) { dp[s] = true; }
        }
    }
    return dp[target];
}

void test(const string& label, const vector<int>& arr, int target, bool want) {
    cout << label << (canSum(arr, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {3, 34, 4, 12, 5, 2}, 9, true);
    test("too big", {3, 34, 4, 12, 5, 2}, 30, false);
    test("no reuse", {5}, 10, false);
    test("target 0", {}, 0, true);
}
