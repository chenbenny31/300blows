// LC 48 Rotate Image: rotate an n x n matrix 90 degrees clockwise in place.
// Follow-up: rotate 90 degrees counter-clockwise in place.
// Trick: clockwise = transpose, then reverse each row
// Recognize: in-place matrix rotation -> compose two reflections.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::reverse;

// Transpose sends (r, c) -> (c, r); the row reverse sends it on to (c, n-1-r),
// which is exactly the clockwise target. T: O(n^2), S: O(1).
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int r = 0; r < n; r++) {
        // c > r only: swapping both halves would undo the transpose
        for (int c = r + 1; c < n; c++) { swap(matrix[r][c], matrix[c][r]); }
    }
    for (auto& row : matrix) { reverse(row.begin(), row.end()); }
}

// follow-up: transpose, then reverse each column ((r, c) -> (n-1-c, r)).
// T: O(n^2), S: O(1).
void rotateCounter(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int r = 0; r < n; r++) {
        for (int c = r + 1; c < n; c++) { swap(matrix[r][c], matrix[c][r]); }
    }
    // reversing the columns = reversing the order of the rows
    reverse(matrix.begin(), matrix.end());
}

void test(const string& label, vector<vector<int>> matrix,
          const vector<vector<int>>& want) {
    rotate(matrix);
    cout << label << (matrix == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("3x3", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {{7, 4, 1}, {8, 5, 2}, {9, 6, 3}});
    test("1x1", {{1}}, {{1}});
}
