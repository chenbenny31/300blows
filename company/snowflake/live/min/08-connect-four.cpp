// B03 · LC 348 variant: board of '.', 'R', 'Y'. The piece at (row, col) was just placed
// by player; return whether it makes 4 in a row (horizontal, vertical, either diagonal).
// Follow-up: drop(col, player) with gravity, returning the landing row.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Matching pieces after (r, c) in direction (dr, dc), at most 3.
int countDir(const vector<string>& board, int r, int c, int dr, int dc, char player) {
    int rows = board.size(), cols = board[0].size();
    int cnt = 0;
    for (int k = 1; k <= 3; k++) {
        int nr = r + k * dr, nc = c + k * dc;
        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { break; }
        if (board[nr][nc] != player) { break; }
        cnt++;
    }
    return cnt;
}

// Only the new piece can create a win, so check the 4 lines through it.
// T: O(1) (4 lines x 6 cells), S: O(1).
bool isWinningMove(const vector<string>& board, int row, int col, char player) {
    // direction d and its opposite: horizontal, vertical, diagonal \, diagonal /
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};
    for (int d = 0; d < 4; d++) {
        // count both sides: the new piece may sit in the middle of the line
        int total = 1 + countDir(board, row, col, dr[d], dc[d], player)
                      + countDir(board, row, col, -dr[d], -dc[d], player);
        if (total >= 4) { return true; }
    }
    return false;
}

// follow-up: gravity drop into the lowest empty cell; -1 if the column is full. T: O(rows).
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
    cout << label << (isWinningMove(board, row, col, 'R') == want ? " PASS\n" : " FAIL\n");
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
