// ByteDance / LC 994: grid of 0 (empty), 1 (fresh), 2 (rotten); each minute fresh
// oranges next to a rotten one (4-directionally) rot. Return the minutes until no fresh
// orange remains, or -1 if impossible.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// Multi-source BFS from all rotten oranges, one layer per minute. T: O(R*C), S: O(R*C).
int orangesRotting(vector<vector<int>> grid) {
    int rows = grid.size(), cols = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    queue<pair<int, int>> q;   // oranges that rotted in the latest minute
    int fresh = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == 2) { q.push({r, c}); }
            if (grid[r][c] == 1) { fresh++; }
        }
    }
    int minutes = 0;
    // stop once fresh hits 0, so the last layer does not count an extra minute
    while (!q.empty() && fresh > 0) {
        minutes++;
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [cr, cc] = q.front();
            q.pop();
            for (auto& d : dirs) {
                int nr = cr + d[0], nc = cc + d[1];
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { continue; }
                if (grid[nr][nc] != 1) { continue; }
                grid[nr][nc] = 2;
                fresh--;
                q.push({nr, nc});
            }
        }
    }
    return fresh == 0 ? minutes : -1;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (orangesRotting(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}, 4);
    test("unreachable", {{2, 1, 1}, {0, 1, 1}, {1, 0, 1}}, -1);
    test("no fresh", {{0, 2}}, 0);
}
