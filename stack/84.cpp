// mono-stack of indices, T: O(n), S: O(n)

#include <vector>
#include <algorithm>

class Solution {
public:
    int largestRectangleArea(std::vector<int>& heights) {
        const int n = static_cast<int>(heights.size());
        std::vector<int> stk; // indices with incre-equal heights
        stk.reserve(n + 1);

        int maxArea = 0;
        for (int i = 0; i <= n; i++) {
            int curH = (i == n) ? 0 : heights[i]; // sentinel bar of height 0
            while (!stk.empty() && heights[stk.back()] >= curH) {
                int h = heights[stk.back()];
                stk.pop_back();
                int left = stk.empty() ? -1 : stk.back(); // first shorter bar on left side
                maxArea = std::max(maxArea, h * (i - left - 1)); // i always has higher bar than left
            }
            stk.push_back(i);
        }
        return maxArea;
    }
};

// nearest-samller dp with jump reuse, T: O(n), S: O(n)

class Solution {
public:
    int largestRectangleArea(std::vector<int>& heights) {
        int n = static_cast<int>(heights.size());
        std::vector<int> left(n, -1); // nearest shorter bar on the left
        std::vector<int> right(n, n); // nearest shorter bar on the right

        for (int i = 0; i < n; i++) {
            int j = i - 1;
            while (j >= 0 && heights[j] >= heights[i]) {
                j = left[j];
            }
            left[i] = j;
        }
        for (int i = n - 1; i >= 0; i--) {
            int j = i + 1;
            while (j < n && heights[j] >= heights[i]) {
                j = right[j];
            }
            right[i] = j;
        }

        int maxArea = 0;
        for (int i = 0; i < n; i++) {
            maxArea = std::max(maxArea, heights[i] * (right[i] - left[i] - 1));
        }
        return maxArea;
    }
};
