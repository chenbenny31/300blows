// ByteDance / LC 317: grid of 0 = empty land, 1 = building, 2 = obstacle. Build a house
// on an empty cell minimizing the total walking distance (4-directional, through empty
// cells only) to all buildings. Return that distance, or -1 if no cell reaches all.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// BFS from one building over empty cells, adding its distances into total and reach.
// T: O(R*C), S: O(R*C).
void bfs(const vector<vector<int>>& grid, int sr, int sc, vector<vector<int>>& total,
         vector<vector<int>>& reach) {
    int R = grid.size(), C = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<vector<int>> dist(R, vector<int>(C, -1));   // dist from (sr, sc); -1 = unseen
    queue<pair<int, int>> q;
    q.push({sr, sc});
    dist[sr][sc] = 0;
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) { continue; }
            // only empty land is walkable; buildings and obstacles block
            if (grid[nr][nc] != 0 || dist[nr][nc] != -1) { continue; }
            dist[nr][nc] = dist[r][c] + 1;
            total[nr][nc] += dist[nr][nc];
            reach[nr][nc]++;
            q.push({nr, nc});
        }
    }
}

// BFS from every building; best empty cell reached by all. T: O(B*R*C), S: O(R*C).
int shortestDistance(const vector<vector<int>>& grid) {
    int R = grid.size(), C = grid[0].size();
    // total[r][c] = sum of distances from the buildings that reach (r, c)
    vector<vector<int>> total(R, vector<int>(C, 0));
    // reach[r][c] = number of buildings with a path to (r, c)
    vector<vector<int>> reach(R, vector<int>(C, 0));
    int buildings = 0;
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (grid[r][c] != 1) { continue; }
            buildings++;
            bfs(grid, r, c, total, reach);
        }
    }
    int best = -1;
    for (int r = 0; r < R; r++) {
        for (int c = 0; c < C; c++) {
            if (grid[r][c] != 0 || reach[r][c] != buildings) { continue; }
            if (best == -1 || total[r][c] < best) { best = total[r][c]; }
        }
    }
    return best;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (shortestDistance(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", {{1, 0, 2, 0, 1}, {0, 0, 0, 0, 0}, {0, 0, 1, 0, 0}}, 7);
    test("one cell", {{1, 0}}, 1);
    test("no land", {{1}}, -1);
}
