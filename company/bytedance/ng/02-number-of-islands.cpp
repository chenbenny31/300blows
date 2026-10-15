// ByteDance (hack2hire) / LC 200: grid of '1' (land) and '0' (water); count islands,
// where an island is land connected horizontally or vertically.
// Follow-up LC 694: count distinct island shapes (same up to translation only).
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::set;

// BFS flood fill per new island, sinking land to '0'. T: O(R*C), S: O(R*C).
int numIslands(vector<vector<char>> grid) {
    int rows = grid.size(), cols = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int islands = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] != '1') { continue; }
            islands++;
            queue<pair<int, int>> q;   // land cells of this island to expand
            q.push({r, c});
            // mark on push, so no cell is queued twice
            grid[r][c] = '0';
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                for (auto& d : dirs) {
                    int nr = cr + d[0], nc = cc + d[1];
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    grid[nr][nc] = '0';
                    q.push({nr, nc});
                }
            }
        }
    }
    return islands;
}

// follow-up: same BFS; the shape key is each cell's offset from the island's first
// cell, in visit order. T: O(R*C log I), S: O(R*C); I = islands.
int numDistinctIslands(vector<vector<char>> grid) {
    int rows = grid.size(), cols = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    set<vector<pair<int, int>>> shapes;   // distinct offset lists seen so far
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] != '1') { continue; }
            // same shape + same scan-first cell + fixed dir order -> same visit order
            vector<pair<int, int>> shape;   // (row - r, col - c) per cell
            queue<pair<int, int>> q;
            q.push({r, c});
            grid[r][c] = '0';
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                shape.push_back({cr - r, cc - c});
                for (auto& d : dirs) {
                    int nr = cr + d[0], nc = cc + d[1];
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    grid[nr][nc] = '0';
                    q.push({nr, nc});
                }
            }
            shapes.insert(shape);
        }
    }
    return shapes.size();
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex2", {{'1', '1', '0', '0', '0'},
                    {'1', '1', '0', '0', '0'},
                    {'0', '0', '1', '0', '0'},
                    {'0', '0', '0', '1', '1'}}, 3);
    test("diagonal", {{'1', '0'}, {'0', '1'}}, 2);
    test("all water", {{'0'}}, 0);
}
