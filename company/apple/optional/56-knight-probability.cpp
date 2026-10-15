// LC 688: a knight on an n x n board starts at (row, column) and makes exactly k moves,
// each uniformly among its 8 moves. Return the probability it is still on the board.
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using std::cout;
using std::string;
using std::vector;
using std::fabs;

// DP over steps: spread each cell's probability to its 8 targets. T: O(k*n^2), S: O(n^2).
double knightProbability(int n, int k, int row, int column) {
    int dr[8] = {1, 1, -1, -1, 2, 2, -2, -2};
    int dc[8] = {2, -2, 2, -2, 1, -1, 1, -1};
    // prob[r][c] = P(knight is on (r, c) after the moves so far)
    vector<vector<double>> prob(n, vector<double>(n, 0));
    prob[row][column] = 1;
    for (int step = 0; step < k; step++) {
        vector<vector<double>> nextProb(n, vector<double>(n, 0));
        for (int r = 0; r < n; r++) {
            for (int c = 0; c < n; c++) {
                for (int d = 0; d < 8; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    // moves off the board are lost probability mass
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                    nextProb[nr][nc] += prob[r][c] / 8;
                }
            }
        }
        prob = nextProb;
    }
    double total = 0;
    for (const auto& line : prob) {
        for (double p : line) { total += p; }
    }
    return total;
}

void test(const string& label, int n, int k, int row, int column, double want) {
    bool ok = fabs(knightProbability(n, k, row, column) - want) < 1e-9;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc ex1", 3, 2, 0, 0, 0.0625);
    test("no moves", 1, 0, 0, 0, 1.0);
    test("1x1 board", 1, 1, 0, 0, 0.0);
}
