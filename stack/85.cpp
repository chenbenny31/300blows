// dp with bookkeeping of shortest bar indices, T: O(mn), S: O(n)

#include <vector>
#include <algorithm>

class Solution {
public:
    int maximalRectangle(std::vector<std::vector<char>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());

        std::vector<int> heights(n, 0); // acc num (height) of 1's at cur row
        // all 1's block, [left[c], right[c])
        std::vector<int> left(n, 0); 
        std::vector<int> right(n, n);
        int maxArea = 0;

        for (int r = 0; r < m; r++) {
            int lastZero = -1; // nearest 0 to the left in row
            for (int c = 0; c < n; c++) {
                if (matrix[r][c] == '1') {
                    heights[c]++;
                    left[c] = std::max(left[c], lastZero + 1); // block starts after zero
                } else {
                    heights[c] = 0;
                    left[c] = 0; // unconstrained, identify for max
                    lastZero = c;
                }
            }

            int nextZero = n; // nearest 0 to the right in row
            for (int c = n - 1; c >= 0; c--) {
                if (matrix[r][c] == '1') {
                    right[c] = std::min(right[c], nextZero);
                } else {
                    right[c] = n; // unconstrained, identify for min
                    nextZero = c;
                }
            }

            for (int c = 0; c < n; c++) {
                maxArea = std::max(maxArea, heights[c] * (right[c] - left[c])); // half-open, len = right - left
            }
        }
        return maxArea;
    }
};

// monotonic stack + recursion, T: O(mn), S: O(n)

class Solution {
public:
    int maximalRectangle(std::vector<std::vector<char>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());
        std::vector<int> heights(n, 0); // acc num (height) of 1's at cur row
        int maxArea = 0;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                heights[c] = (matrix[r][c] == '1') ? heights[c] + 1 : 0; // update before scoring this row
            }
            maxArea = std::max(maxArea, maxRow(heights));
        }
        return maxArea;
    }

private:
    int maxRow(const std::vector<int>& heights) {
        int n = static_cast<int>(heights.size());
        std::vector<int> stk; // indices with heights strictly increasing from bottom
        stk.reserve(n + 1);
        
        int maxArea = 0;
        for (int i = 0; i <= n; i++) { // i=n to flush stack
            int curH = (i == n) ?  0 : heights[i]; // sentinel bar of height 0 to flush the stack
            while (!stk.empty() && heights[stk.back()] >= curH) { // equal also trigger computing visited maxArea
                int h = heights[stk.back()];
                stk.pop_back();
                int left = stk.empty() ? -1 : stk.back();
                maxArea = std::max(maxArea, h * (i - left -1));
            }
            stk.push_back(i);
        }
        return maxArea;
    }
};
