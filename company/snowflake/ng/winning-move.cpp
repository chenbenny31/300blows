#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// match pieces after (r, c) in direction (dr, dc)
int countDir(const vector<string>& board,
             int r, int c, int dr, int dc, char player) {
    int m = board.size();
    int n = board[0].size();
    int count = 0;

    for (int k = 1; k <= 3; k++) {
        int nr = r + k * dr;
        int nc = c + k * dc;
        if (nr < 0 || nr >= m || nc < 0 || nc >= n) { break; }
        if (board[nr][nc] != player) { break; }
        count++;
    }
    return count;
}

// T: O(1), S: O(1)
bool isWinningMove(const vector<string>& board, int row, int col, char player) {
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};

    for (int d = 0; d < 4; d++) {
        int total = 1
                  + countDir(board, row, col, dr[d], dc[d], player)
                  + countDir(board, row, col, -dr[d], -dc[d], player);
        if (total >= 4) { return true; }
    }
    return false;
}

// follow-up: gravity drop into the lowest empty cell, -1 if col full, T: O(m)
int drop(vector<string>& board, int col, char player) {
    int rows = board.size();

    for (int r = rows - 1; r >= 0; r--) {
        if (board[r][col] == '.') {
            board[r][col] = player;
            return r;
        }
    }
    return -1;
}

void test(const string& label, const vector<string>& board, int row, int col, bool want) {
    bool got = isWinningMove(board, row, col, 'R');
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle of row", {"....",
                           "....",
                           "....",
                           "RRRR"}, 3, 2, true);
    test("diagonal /", {"...R",
                        "..R.",
                        ".R..",
                        "R..."}, 2, 1, true);
    test("only three", {"....",
                        "....",
                        "....",
                        "RRR."}, 3, 2, false);
}
