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

// bfs, T: O(V+E), S: O(V)
int shortestPath(const vector<vector<int>>& grid,
                 int sr, int sc, int tr, int tc) {
    if (grid.empty() || grid[sr][sc] == 1 || grid[tr][tc] == 1) {
        return -1;
    }

    int m = grid.size();
    int n = grid[0].size();
    int dr[] = {-1, +1, 0, 0};
    int dc[] = {0, 0, +1, -1};

    // dist[r][c] = min moves from start to (r, c), -1 for un-reachable
    vector<vector<int>> dist(m, vector<int>(n, -1));
    queue<pair<int, int>> q;
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
            dist[nr][nc] = dist[r][c] + 1; // mark before enqueue
            q.push({nr, nc});
        }
    }
    return -1;
}
// follow-up: bfs, store path, T: O(V+E), S: O(V)
vector<pair<int, int>> shortestPathCells(const vector<vector<int>>& grid,
                                         int sr, int sc, int tr, int tc) {
    if (grid.empty() || grid[sr][sc] == 1 || grid[tr][tc] == 1) { return {}; }
    int m = grid.size();
    int n = grid[0].size();
    int dr[] = {-1, +1, 0, 0};
    int dc[] = {0, 0, -1, +1};
    // parent[r][c] = cell reached (r, c) from, {-1, -1} = no-reachable
    vector<vector<pair<int, int>>> parent(m, vector<pair<int, int>>(n, {-1, -1}));
    queue<pair<int, int>> q;
    parent[sr][sc] = {sr, sc};
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
    vector<pair<int, int>> path;
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
    cout << label
         << (shortestPath(g, sr, sc, tr, tc) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> g = {
        {0, 0, 0},
        {1, 1, 0},
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0}
    };
    test("detour", g, 0, 0, 4, 0, 8);
    test("sealed", g, 0, 0, 4, 2, -1);
    test("wall", g, 0, 0, 1, 0, -1);
    test("same", g, 0, 0, 0, 0, 0);
}
