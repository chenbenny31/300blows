// ByteDance (hack2hire) / LC 778: n x n grid of distinct elevations 0..n*n-1. At time t
// you can swim between 4-adjacent cells of elevation <= t. Return the min t to get from
// (0, 0) to (n-1, n-1).
// Follow-up: binary search on t + BFS.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;
using std::greater;
using std::max;

// Dijkstra where a path's cost is its max elevation. T: O(n^2 log n), S: O(n^2).
int swimInWater(const vector<vector<int>>& grid) {
    int n = grid.size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<vector<bool>> seen(n, vector<bool>(n, false));   // cell already pushed
    // min-heap of (max elevation on the path, r * n + c)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    heap.push({grid[0][0], 0});
    seen[0][0] = true;
    while (!heap.empty()) {
        auto [t, cell] = heap.top();
        heap.pop();
        int r = cell / n, c = cell % n;
        if (r == n - 1 && c == n - 1) { return t; }
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n || seen[nr][nc]) { continue; }
            // mark on push: pops come in non-decreasing t, so a later push is no better
            seen[nr][nc] = true;
            heap.push({max(t, grid[nr][nc]), nr * n + nc});
        }
    }
    return -1;
}

// BFS through cells of elevation <= t. T: O(n^2), S: O(n^2).
bool canReach(const vector<vector<int>>& grid, int t) {
    int n = grid.size();
    if (grid[0][0] > t) { return false; }
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<vector<bool>> seen(n, vector<bool>(n, false));   // cell already queued
    queue<pair<int, int>> q;
    q.push({0, 0});
    seen[0][0] = true;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == n - 1 && c == n - 1) { return true; }
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n || seen[nr][nc]) { continue; }
            if (grid[nr][nc] > t) { continue; }
            seen[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return false;
}

// follow-up: canReach is monotone in t; binary search the smallest t.
// T: O(n^2 log n), S: O(n^2).
int swimInWaterBinary(const vector<vector<int>>& grid) {
    int n = grid.size();
    int lo = grid[0][0], hi = n * n - 1;   // at t = n*n - 1 every cell is open
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (canReach(grid, mid)) { hi = mid; } else { lo = mid + 1; }
    }
    return lo;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (swimInWater(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{0, 2}, {1, 3}}, 3);
    test("lc ex2", {{0, 1, 2, 3, 4},
                    {24, 23, 22, 21, 5},
                    {12, 13, 14, 15, 16},
                    {11, 17, 18, 19, 20},
                    {10, 9, 8, 7, 6}}, 16);
}
