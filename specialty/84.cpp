// LC 84 Largest Rectangle in Histogram: bars of width 1 with the given heights;
// return the area of the largest rectangle inside the histogram.
// Trick: increasing stack; a popped bar spans (new top, i); sentinel 0 flushes the end
// Recognize: "nearest smaller bar on both sides" for every element.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Monotonic increasing stack of indices; a bar's rectangle ends at the first lower bar.
// T: O(n), S: O(n).
int largestRectangleArea(vector<int> heights) {
    heights.push_back(0);   // sentinel: flushes every bar left on the stack
    int n = heights.size();
    vector<int> stk;        // indices with strictly increasing heights
    int best = 0;
    for (int i = 0; i < n; i++) {
        while (!stk.empty() && heights[stk.back()] >= heights[i]) {
            int h = heights[stk.back()];
            stk.pop_back();
            // the bar below on the stack is the first lower bar to the left
            int left = stk.empty() ? -1 : stk.back();
            best = max(best, h * (i - left - 1));
        }
        stk.push_back(i);
    }
    return best;
}

void test(const string& label, const vector<int>& heights, int want) {
    cout << label << (largestRectangleArea(heights) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {2, 1, 5, 6, 2, 3}, 10);
    test("increasing", {2, 4}, 4);
    test("single", {1}, 1);
}
