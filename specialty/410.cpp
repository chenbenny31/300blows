// LC 410 Split Array Largest Sum: split non-negative nums into k non-empty contiguous
// parts minimizing the largest part sum.
// Trick: binary search the answer in [max, sum]; greedily count pieces with sum <= mid
// Recognize: "minimize the maximum" + a monotone feasibility check
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
using std::cout;
using std::string;
using std::vector;
using std::max_element;
using std::accumulate;

// greedy: fill each piece as long as it stays <= cap. T: O(n), S: O(1).
int piecesNeeded(const vector<int>& nums, long long cap) {
    int pieces = 1;
    long long cur = 0;   // sum of the current piece
    for (int x : nums) {
        if (cur + x > cap) {
            pieces++;
            cur = 0;
        }
        cur += x;
    }
    return pieces;
}

// A bigger cap never needs more pieces, and <= k pieces can be split into exactly k.
// T: O(n log(sum)), S: O(1).
int splitArray(const vector<int>& nums, int k) {
    // answer lies in [largest element, total sum]
    long long lo = *max_element(nums.begin(), nums.end());
    long long hi = accumulate(nums.begin(), nums.end(), 0LL);
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (piecesNeeded(nums, mid) <= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return (int)lo;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (splitArray(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {7, 2, 5, 10, 8}, 2, 18);
    test("lc ex2", {1, 2, 3, 4, 5}, 2, 9);
    test("k = n", {1, 4, 4}, 3, 4);
}
