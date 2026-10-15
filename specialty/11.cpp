// LC 11 Container With Most Water: pick two lines height[i], height[j]; maximize
// (j - i) * min(height[i], height[j]).
// Trick: two pointers from both ends; always move the shorter side inward
// Recognize: maximize width * min(pair) over all pairs in O(n).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;

// Every pair using the shorter line with a narrower width is capped by that line,
// so it can never beat the current area: discard the shorter line. T: O(n), S: O(1).
int maxArea(const vector<int>& height) {
    int lo = 0, hi = (int)height.size() - 1;
    int best = 0;
    while (lo < hi) {
        best = max(best, (hi - lo) * min(height[lo], height[hi]));
        if (height[lo] < height[hi]) {
            lo++;
        } else {
            hi--;
        }
    }
    return best;
}

void test(const string& label, const vector<int>& height, int want) {
    cout << label << (maxArea(height) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {1, 8, 6, 2, 5, 4, 8, 3, 7}, 49);
    test("two lines", {1, 1}, 1);
}
