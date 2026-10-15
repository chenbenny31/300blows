// A07 · LC 1091 variant: fewest 4-directional moves from (sr, sc) to (tr, tc)
// on a 0/1 grid (1 = wall); -1 if unreachable.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::queue;
using std::reverse;

// BFS. T: O(V+E), S: O(V); V = cells, E <= 4V.
// Each cell is queued once and checks 4 neighbours.
int shortestPath(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
    if (grid.empty() || grid[sr][sc] == 1 || grid[tr][tc] == 1) { return -1; }
    int m = grid.size(), n = grid[0].size();
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    // dist[r][c] = fewest moves start -> (r,c); -1 = not reached (doubles as visited)
    vector<vector<int>> dist(m, vector<int>(n, -1));
    queue<pair<int, int>> q;   // cells to expand, in non-decreasing dist
    dist[sr][sc] = 0;
    q.push({sr, sc});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == tr && c == tc) { return dist[r][c]; }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (grid[nr][nc] == 1 || dist[nr][nc] != -1) { continue; }
            // mark when pushed, so no cell is queued twice
            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr, nc});
        }
    }
    return -1;
}

// follow-up: same BFS storing each cell's parent, then walk back from target.
// T: O(V+E), S: O(V).
vector<pair<int, int>> shortestPathCells(const vector<vector<int>>& grid,
                                         int sr, int sc, int tr, int tc) {
    if (grid.empty() || grid[sr][sc] == 1 || grid[tr][tc] == 1) { return {}; }
    int m = grid.size(), n = grid[0].size();
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    // parent[r][c] = cell we reached (r,c) from; {-1,-1} = not reached
    vector<vector<pair<int, int>>> parent(m, vector<pair<int, int>>(n, {-1, -1}));
    queue<pair<int, int>> q;
    parent[sr][sc] = {sr, sc};
    q.push({sr, sc});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == tr && c == tc) { break; }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (grid[nr][nc] == 1 || parent[nr][nc].first != -1) { continue; }
            parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }
    if (parent[tr][tc].first == -1) { return {}; }
    vector<pair<int, int>> path;   // cells from target back to start, reversed at the end
    pair<int, int> cur = {tr, tc};
    while (cur != pair<int, int>{sr, sc}) {
        path.push_back(cur);
        cur = parent[cur.first][cur.second];
    }
    path.push_back({sr, sc});
    reverse(path.begin(), path.end());
    return path;
}

void test(const string& label, const vector<vector<int>>& g,
          int sr, int sc, int tr, int tc, int want) {
    cout << label << (shortestPath(g, sr, sc, tr, tc) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> g = {{0, 0, 0},
                             {1, 1, 0},
                             {0, 0, 0},
                             {0, 0, 1},
                             {0, 1, 0}};
    test("detour", g, 0, 0, 4, 0, 8);    // must go around the wall (Manhattan would be 4)
    test("sealed", g, 0, 0, 4, 2, -1);   // (4,2) only touches open cells diagonally
    test("wall", g, 0, 0, 1, 0, -1);     // target is a wall
    test("same", g, 0, 0, 0, 0, 0);      // start == target
}
