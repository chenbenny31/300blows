// LC 54: return all elements of an m x n matrix in clockwise spiral order.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Peel one layer at a time with four shrinking boundaries. T: O(R*C), S: O(1) extra.
vector<int> spiralOrder(const vector<vector<int>>& matrix) {
    vector<int> result;
    if (matrix.empty()) { return result; }
    // unvisited rows are [top, bottom], unvisited columns are [left, right]
    int top = 0, bottom = matrix.size() - 1;
    int left = 0, right = matrix[0].size() - 1;
    while (top <= bottom && left <= right) {
        for (int c = left; c <= right; c++) { result.push_back(matrix[top][c]); }
        top++;
        for (int r = top; r <= bottom; r++) { result.push_back(matrix[r][right]); }
        right--;
        // guards: a single remaining row or column must not be walked back
        if (top <= bottom) {
            for (int c = right; c >= left; c--) { result.push_back(matrix[bottom][c]); }
            bottom--;
        }
        if (left <= right) {
            for (int r = bottom; r >= top; r--) { result.push_back(matrix[r][left]); }
            left++;
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& matrix,
          const vector<int>& want) {
    cout << label << (spiralOrder(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {1, 2, 3, 6, 9, 8, 7, 4, 5});
    test("wide", {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}},
         {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
    test("one column", {{1}, {2}, {3}}, {1, 2, 3});
}
