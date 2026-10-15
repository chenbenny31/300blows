// LC tag · LC 42: height[i] = elevation of a width-1 bar; return how much rain water
// is trapped between the bars.
// Follow-up: O(1) extra space.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Water above bar i = min(tallest left, tallest right) - height[i]. T: O(n), S: O(n).
long long trap(const vector<int>& height) {
    int n = height.size();
    if (n == 0) { return 0; }
    vector<int> leftMax(n);    // leftMax[i] = tallest bar in height[0..i]
    vector<int> rightMax(n);   // rightMax[i] = tallest bar in height[i..n-1]
    leftMax[0] = height[0];
    for (int i = 1; i < n; i++) { leftMax[i] = max(leftMax[i - 1], height[i]); }
    rightMax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; i--) { rightMax[i] = max(rightMax[i + 1], height[i]); }
    long long water = 0;
    for (int i = 0; i < n; i++) { water += min(leftMax[i], rightMax[i]) - height[i]; }
    return water;
}

// follow-up: two pointers; the lower side's max bounds its water. T: O(n), S: O(1).
long long trapTwoPointers(const vector<int>& height) {
    int lo = 0, hi = (int)height.size() - 1;
    int leftMax = 0;    // tallest bar in height[0..lo]
    int rightMax = 0;   // tallest bar in height[hi..n-1]
    long long water = 0;
    while (lo < hi) {
        // the shorter side is the limit: the other side has a wall at least as tall
        if (height[lo] < height[hi]) {
            leftMax = max(leftMax, height[lo]);
            water += leftMax - height[lo];
            lo++;
        } else {
            rightMax = max(rightMax, height[hi]);
            water += rightMax - height[hi];
            hi--;
        }
    }
    return water;
}

void test(const string& label, const vector<int>& height, long long want) {
    cout << label << (trap(height) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6);
    test("lc ex2", {4, 2, 0, 3, 2, 5}, 9);
    test("rising", {1, 2, 3}, 0);
    test("empty", {}, 0);
}
