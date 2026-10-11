// m x n grid of 0 (open) / 1 (wall), shortest path from (sr, sc) to (tr, tc) without stepping on a 1
// return the number of moves, -1 if unreachable (Q32, C09 NG round 1)
// LC1091 variant: top-left to bottom-right, 8 directions, length counted in cells

// 4 or 8 directions?
// distance in moves (edges) or cells on the path?
// start or target on a wall? start == target?
// unreachable return value?
// grid can be empty? m, n bounds?

#include <vector>
#include <queue>
#include <utility>
#include <cassert>
#include <iostream>

using std::vector;

class GridPathFinder {
public:
    // Q32: any start -> target, 4 directions, counts moves
    // BFS, every move costs 1 so the first time a cell is reached is its shortest distance
    // T: O(m*n), S: O(m*n), grid not modified
    int shortestPath(const vector<vector<int>>& grid, int sr, int sc, int tr, int tc) {
        int m = static_cast<int>(grid.size());
        if (m == 0 || grid[sr][sc] == 1 || grid[tr][tc] == 1) {
            return -1;
        }
        int n = static_cast<int>(grid[0].size());
        vector<int> dr = {-1, 1, 0, 0}; // up, down, left, right
        vector<int> dc = {0, 0, -1, 1};

        vector<vector<int>> dist(m, vector<int>(n, -1)); // cell -> moves from start, -1 = not visited
        std::queue<std::pair<int, int>> q; // cells in BFS order, distance non-decreasing
        dist[sr][sc] = 0;
        q.push({sr, sc});
        while (!q.empty()) {
            int r = q.front().first;
            int c = q.front().second;
            q.pop();
            if (r == tr && c == tc) {
                return dist[r][c];
            }
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) {
                    continue;
                }
                if (grid[nr][nc] == 1 || dist[nr][nc] != -1) {
                    continue;
                }
                dist[nr][nc] = dist[r][c] + 1; // mark on push, else a cell is queued many times
                q.push({nr, nc});
            }
        }
        return -1;
    }

    // LC1091: (0, 0) -> (n-1, n-1) in an n x n grid, 8 directions, counts cells on the path
    // same BFS, start cell counts as 1, T: O(n^2), S: O(n^2)
    int shortestPathBinaryMatrix(const vector<vector<int>>& grid) {
        int n = static_cast<int>(grid.size());
        if (n == 0 || grid[0][0] == 1 || grid[n - 1][n - 1] == 1) {
            return -1;
        }
        vector<int> dr = {-1, -1, -1, 0, 0, 1, 1, 1}; // all 8 neighbours
        vector<int> dc = {-1, 0, 1, -1, 1, -1, 0, 1};

        vector<vector<int>> dist(n, vector<int>(n, -1)); // cell -> cells on the path so far, -1 = not visited
        std::queue<std::pair<int, int>> q;
        dist[0][0] = 1; // the start cell itself
        q.push({0, 0});
        while (!q.empty()) {
            int r = q.front().first;
            int c = q.front().second;
            q.pop();
            if (r == n - 1 && c == n - 1) {
                return dist[r][c];
            }
            for (int d = 0; d < 8; d++) {
                int nr = r + dr[d];
                int nc = c + dc[d];
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
                    continue;
                }
                if (grid[nr][nc] == 1 || dist[nr][nc] != -1) {
                    continue;
                }
                dist[nr][nc] = dist[r][c] + 1;
                q.push({nr, nc});
            }
        }
        return -1;
    }
};

int main() {
    GridPathFinder gp;
    vector<vector<int>> wall = {{0, 0, 0},
                                {1, 1, 0},
                                {0, 0, 0}};

    assert(gp.shortestPath(wall, 0, 0, 2, 0) == 6); // detour around the wall
    assert(gp.shortestPath(wall, 1, 2, 1, 2) == 0); // start == target
    assert(gp.shortestPath({{0, 1}, {1, 0}}, 0, 0, 1, 1) == -1); // sealed off in 4 directions
    assert(gp.shortestPath({{1, 0}}, 0, 0, 0, 1) == -1); // start on a wall

    assert(gp.shortestPathBinaryMatrix({{0, 1}, {1, 0}}) == 2); // LC example, diagonal, counts cells
    assert(gp.shortestPathBinaryMatrix({{0, 0, 0}, {1, 1, 0}, {1, 1, 0}}) == 4); // LC example
    assert(gp.shortestPathBinaryMatrix({{1, 0, 0}, {1, 1, 0}, {1, 1, 0}}) == -1); // blocked start
    assert(gp.shortestPathBinaryMatrix({{0}}) == 1); // n = 1, start is the target

    std::cout << "all tests passed\n";
    return 0;
}
