// ByteDance / LC 934: n x n grid of 1 (land) and 0 (water) with exactly two islands
// (4-directionally connected). Return the min number of 0s to flip to connect them.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// DFS: mark the first island 2 and queue its cells. T: O(n^2), S: O(n^2).
void markIsland(vector<vector<int>>& grid, int r, int c, queue<pair<int, int>>& q) {
    int n = grid.size();
    if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) { return; }
    grid[r][c] = 2;
    q.push({r, c});
    markIsland(grid, r + 1, c, q);
    markIsland(grid, r - 1, c, q);
    markIsland(grid, r, c + 1, q);
    markIsland(grid, r, c - 1, q);
}

// Mark island one, then multi-source BFS by layers until reaching a 1.
// T: O(n^2), S: O(n^2).
int shortestBridge(vector<vector<int>> grid) {
    int n = grid.size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    queue<pair<int, int>> q;   // frontier cells, all at the same distance from island one
    for (int r = 0; r < n && q.empty(); r++) {
        for (int c = 0; c < n && q.empty(); c++) {
            if (grid[r][c] == 1) { markIsland(grid, r, c, q); }
        }
    }
    int flips = 0;
    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [cr, cc] = q.front();
            q.pop();
            for (auto& d : dirs) {
                int nr = cr + d[0], nc = cc + d[1];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                if (grid[nr][nc] == 1) { return flips; }
                if (grid[nr][nc] == 2) { continue; }
                // mark water on push so it is flipped at most once
                grid[nr][nc] = 2;
                q.push({nr, nc});
            }
        }
        flips++;
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (shortestBridge(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diagonal", {{0, 1}, {1, 0}}, 1);
    test("corners", {{0, 1, 0}, {0, 0, 0}, {0, 0, 1}}, 2);
    test("ring", {{1, 1, 1, 1, 1},
                  {1, 0, 0, 0, 1},
                  {1, 0, 1, 0, 1},
                  {1, 0, 0, 0, 1},
                  {1, 1, 1, 1, 1}}, 1);
}
