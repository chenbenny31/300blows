// A07 · LC 1091 variant: 0/1 grid, shortest number of 4-directional moves from (sr, sc) to (tr, tc) without stepping on a 1, -1 if unreachable.
// Follow-up: return one shortest path as a list of cells.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::pair;

class GridPathFinder {
public:
    // BFS, all moves cost 1 so the first discovery of a cell is its shortest distance, T: O(m*n), S: O(m*n)
    int shortestPath(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
        if (grid.empty() || grid[0].empty()) { return -1; }
        if (grid[sr][sc] == 1 || grid[tr][tc] == 1) { return -1; }
        int m = grid.size();
        int n = grid[0].size();
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        vector<vector<int>> dist(m, vector<int>(n, -1));   // cell -> moves from start, -1 = not seen
        std::queue<pair<int, int>> q;                      // cells in non-decreasing distance order
        dist[sr][sc] = 0;
        q.push({sr, sc});
        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();
            if (r == tr && c == tc) { return dist[r][c]; }
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (grid[nr][nc] == 1 || dist[nr][nc] != -1) { continue; }
                dist[nr][nc] = dist[r][c] + 1;             // mark on push, or a cell is queued many times
                q.push({nr, nc});
            }
        }
        return -1;
    }

    // BFS with parent pointers, walk back from target, T: O(m*n), S: O(m*n)
    vector<pair<int, int>> shortestPathCells(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
        if (grid.empty() || grid[0].empty()) { return {}; }
        if (grid[sr][sc] == 1 || grid[tr][tc] == 1) { return {}; }
        int m = grid.size();
        int n = grid[0].size();
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        vector<vector<pair<int, int>>> parent(m, vector<pair<int, int>>(n, {-1, -1}));   // cell -> cell it was reached from, {-1,-1} = not seen
        std::queue<pair<int, int>> q;
        parent[sr][sc] = {sr, sc};                         // start points to itself, doubles as visited
        q.push({sr, sc});
        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();
            if (r == tr && c == tc) { break; }
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (grid[nr][nc] == 1 || parent[nr][nc].first != -1) { continue; }
                parent[nr][nc] = {r, c};
                q.push({nr, nc});
            }
        }
        if (parent[tr][tc].first == -1) { return {}; }
        vector<pair<int, int>> path;                       // target back to start, reversed at the end
        pair<int, int> cur = {tr, tc};
        while (cur != pair<int, int>{sr, sc}) {
            path.push_back(cur);
            cur = parent[cur.first][cur.second];
        }
        path.push_back({sr, sc});
        std::reverse(path.begin(), path.end());
        return path;
    }
};

// path starts and ends right, every step is one open 4-neighbour move
bool isValidPath(const vector<vector<int>>& grid, const vector<pair<int, int>>& path,
                 int sr, int sc, int tr, int tc) {
    if (path.empty() || path.front() != pair<int, int>{sr, sc} || path.back() != pair<int, int>{tr, tc}) { return false; }
    int k = path.size();
    for (int i = 0; i < k; i++) {
        auto [r, c] = path[i];
        if (grid[r][c] == 1) { return false; }
        if (i > 0) {
            int dr = r - path[i - 1].first;
            int dc = c - path[i - 1].second;
            if (dr * dr + dc * dc != 1) { return false; }   // exactly one of dr, dc is +-1
        }
    }
    return true;
}

bool test(const string& label, const vector<vector<int>>& grid, int sr, int sc, int tr, int tc, int expected) {
    GridPathFinder gp;
    int got = gp.shortestPath(grid, sr, sc, tr, tc);
    vector<pair<int, int>> path = gp.shortestPathCells(grid, sr, sc, tr, tc);
    bool pathOk = expected == -1 ? path.empty()
                                 : (int)path.size() == expected + 1 && isValidPath(grid, path, sr, sc, tr, tc);
    if (got == expected && pathOk) { return true; }
    cout << "FAIL " << label << ": got " << got << " (path cells " << path.size()
         << "), expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    vector<vector<int>> wall = {{0, 0, 0},
                                {1, 1, 0},
                                {0, 0, 0}};
    ok &= test("detour", wall, 0, 0, 2, 0, 6);                                    // target: around the wall
    ok &= test("same cell", wall, 1, 2, 1, 2, 0);                                 // start == target
    ok &= test("single cell", {{0}}, 0, 0, 0, 0, 0);                              // smallest input
    ok &= test("no diagonal", {{0, 1}, {1, 0}}, 0, 0, 1, 1, -1);                  // only diagonal link
    ok &= test("start blocked", {{1, 0}}, 0, 0, 0, 1, -1);                        // start on a wall
    ok &= test("target blocked", {{0, 1}}, 0, 0, 0, 1, -1);                       // target on a wall
    ok &= test("empty grid", {}, 0, 0, 0, 0, -1);                                 // no rows
    ok &= test("open 3x4", {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 2, 3, 0, 0, 5);   // Manhattan when open
    ok &= test("snake", {{0, 0, 0},
                         {1, 1, 0},
                         {0, 0, 0},
                         {0, 1, 1},
                         {0, 0, 0}}, 0, 0, 4, 2, 10);                             // long forced path, many wrong turns
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
