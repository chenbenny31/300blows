#include <algorithm>
#include <cassert>
#include <utility>
#include <queue>
#include <vector>

using std::vector;
using std::pair;

// T: O(V+E), S: O(V)
vector<pair<int, int>> shortestPath(const vector<vector<int>>& grid,
                                    int sr, int sc, int tr, int tc) {
    if (grid.empty() || grid[0].empty()) { return {}; }
    if (grid[sr][sc] == 1 || grid[tr][tc] == 1) { return {}; }

    int m = grid.size(), n = grid[0].size();
    pair<int, int> start = {sr, sc};
    pair<int, int> target = {tr, tc};
    pair<int, int> unseen = {-1, -1};

    vector<vector<pair<int, int>>> parent(m, vector<pair<int, int>>(n, unseen));
    std::queue<pair<int, int>> q;
    q.push(start);
    parent[sr][sc] = start;

    int dr[] = {0, 0, -1, +1};
    int dc[] = {+1, -1, 0, 0};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == tr && c == tc) {
            vector<pair<int, int>> path;
            for (auto p = target; p != start; p = parent[p.first][p.second]) {
                path.push_back(p);
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (grid[nr][nc] == 1 || parent[nr][nc] != unseen) { continue; }

            parent[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }
    return {};
}

int main() {
    vector<vector<int>> grid = {
        {0, 0, 0},
        {1, 1, 0},
        {0, 0, 0}
    };

    vector<pair<int, int>> want = {
        {0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}, {2, 0}
    };

    auto path = shortestPath(grid, 0, 0, 2, 0);
    assert(path == want);

    // start == target: path contains only start
    // unreachable: empty path
    // blocked target: empty path
    // empty grid: empty path

}
