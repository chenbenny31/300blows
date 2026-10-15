#include <cassert>
#include <queue>
#include <utility>
#include <vector>

using std::vector;
using std::pair;
using std::queue;

// T: O(V+E), S: O(V)
int shortestPath(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
    if (grid.empty() || grid[0].empty()) { return -1; }
    if (grid[sr][sc] == 1 || grid[tr][tc] == 1) { return -1; }

    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> dist(m, vector<int>(n, -1));
    queue<pair<int, int>> q;

    q.push({sr, sc});
    dist[sr][sc] = 0; // dist for mark is-visited and target values bookkeeping
    int dr[] = {0, 0, -1, +1};
    int dc[] = {+1, -1, 0, 0};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == tr && c == tc) { return dist[r][c]; }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (grid[nr][nc] == 1 || dist[nr][nc] != -1) { continue; }
            dist[nr][nc] = dist[r][c] + 1; // make {nr, nc} before entering
            q.push({nr, nc});
        }
    }
    return -1;
}

int main() {
    vector<vector<int>> grid = {
        {0, 0, 0},
        {1, 1, 0},
        {0, 0, 0}
    };

    assert(shortestPath(grid, 0, 0, 2, 0) == 6);
    // same start/target
    // no diagonal/blocked-path
    // blocked at start/target
    // empty grid/row
}
