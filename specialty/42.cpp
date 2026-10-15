// LC 42 Trapping Rain Water: height[i] = elevation of a width-1 bar; return how much
// rain water is trapped between the bars.
// Trick: two pointers; the lower side's running max bounds its water
// Recognize: water at i = min(max on left, max on right) - height[i].
// Follow-up: solve it with a monotonic stack.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Two pointers; the lower side's max bounds its water. T: O(n), S: O(1).
int trap(const vector<int>& height) {
    int lo = 0, hi = (int)height.size() - 1;
    int leftMax = 0;    // tallest bar in height[0..lo]
    int rightMax = 0;   // tallest bar in height[hi..n-1]
    int water = 0;
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

// follow-up: decreasing stack; a taller bar closes the basin above the popped bottom.
// T: O(n), S: O(n).
int trapStack(const vector<int>& height) {
    vector<int> stk;   // indices of bars with non-increasing heights
    int water = 0;
    int n = height.size();
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && height[i] > height[stk.back()]) {
            int bottom = stk.back();
            stk.pop_back();
            if (stk.empty()) { break; }   // no left wall: water spills out
            int left = stk.back();
            int depth = min(height[left], height[i]) - height[bottom];
            water += depth * (i - left - 1);
        }
        stk.push_back(i);
    }
    return water;
}

void test(const string& label, const vector<int>& height, int want) {
    cout << label << (trap(height) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6);
    test("lc ex2", {4, 2, 0, 3, 2, 5}, 9);
    test("empty", {}, 0);
}
