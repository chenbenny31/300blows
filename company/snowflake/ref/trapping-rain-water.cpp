// bar heights, width 1 each: how much rain water is trapped between the bars (LC42)

// heights non-negative?
// empty input or fewer than 3 bars?
// total can exceed int? (n * max height)

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;

class RainWater {
public:
    // 1: water above bar i = min(tallest on its left, tallest on its right) - height[i]
    // precompute both maxima, T: O(n), S: O(n)
    long long trap_1(const vector<int>& height) {
        int n = static_cast<int>(height.size());
        if (n == 0) {
            return 0;
        }
        vector<int> leftMax(n); // i -> tallest bar in [0, i]
        vector<int> rightMax(n); // i -> tallest bar in [i, n-1]
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = std::max(leftMax[i - 1], height[i]);
        }
        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--) {
            rightMax[i] = std::max(rightMax[i + 1], height[i]);
        }
        long long water = 0;
        for (int i = 0; i < n; i++) {
            water += std::min(leftMax[i], rightMax[i]) - height[i];
        }
        return water;
    }

    // 2: two pointers; the lower side is the limit, the other side surely has a wall at least as tall
    // T: O(n), S: O(1)
    long long trap_2(const vector<int>& height) {
        int l = 0;
        int r = static_cast<int>(height.size()) - 1;
        int leftMax = 0; // tallest in [0, l]
        int rightMax = 0; // tallest in [r, n-1]
        long long water = 0;
        while (l < r) {
            if (height[l] < height[r]) {
                leftMax = std::max(leftMax, height[l]);
                water += leftMax - height[l]; // right side has height[r] > height[l], so leftMax decides
                l++;
            } else {
                rightMax = std::max(rightMax, height[r]);
                water += rightMax - height[r];
                r--;
            }
        }
        return water;
    }
};

int main() {
    RainWater rw;

    assert(rw.trap_1({3, 0, 2, 0, 4}) == 7); // target: 3 + 1 + 3
    assert(rw.trap_2({3, 0, 2, 0, 4}) == 7);
    assert(rw.trap_1({2, 0, 2}) == 2); // edge: equal walls
    assert(rw.trap_2({2, 0, 2}) == 2);
    assert(rw.trap_1({1, 2, 3}) == 0); // fault: rising bars, no right wall
    assert(rw.trap_2({1, 2, 3}) == 0);
    assert(rw.trap_1({}) == 0); // null
    assert(rw.trap_2({}) == 0);

    std::cout << "all tests passed\n";
    return 0;
}
