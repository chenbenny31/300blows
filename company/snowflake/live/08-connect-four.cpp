// B03 · LC 348 variant: board of '.', 'R', 'Y'. The piece at (row, col) was just placed by `player`;
// return whether it completes 4 in a row horizontally, vertically or on either diagonal.
// Follow-up: drop(col, player) on a gravity board, returning the landing row and whether that move wins.

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

class ConnectFour {
public:
    // count matching pieces outward in both directions along 4 axes, T: O(1) (at most 4 * 6 cells), S: O(1)
    bool isWinningMove(const vector<string>& board, int row, int col, char player) {
        int dr[] = {0, 1, 1, 1};           // axis d and its opposite: horizontal, vertical, diagonal \, diagonal /
        int dc[] = {1, 0, 1, -1};
        for (int d = 0; d < 4; d++) {
            int total = 1 + countDir(board, row, col, dr[d], dc[d], player)
                          + countDir(board, row, col, -dr[d], -dc[d], player);   // the new piece plus both sides
            if (total >= 4) { return true; }
        }
        return false;
    }

    // gravity version: lowest empty cell in col; returns landing row, -1 if the column is full
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

private:
    // matching pieces after (r, c) in direction (dr, dc), stopping at the edge or a different cell, at most 3
    int countDir(const vector<string>& board, int r, int c, int dr, int dc, char player) {
        int rows = board.size();
        int cols = board[0].size();
        int cnt = 0;
        for (int k = 1; k <= 3; k++) {
            int nr = r + k * dr;
            int nc = c + k * dc;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) { break; }
            if (board[nr][nc] != player) { break; }
            cnt++;
        }
        return cnt;
    }
};

bool test(const string& label, const vector<string>& board, int row, int col, char player, bool expected) {
    ConnectFour cf;
    bool got = cf.isWinningMove(board, row, col, player);
    if (got == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << ", expected " << expected << "\n";
    return false;
}

bool check(bool cond, const string& label) {
    if (cond) { return true; }
    cout << "FAIL " << label << "\n";
    return false;
}

// drop sequence on an empty 6x7 board; Y builds a vertical four in column 3
bool testDropGame() {
    bool ok = true;
    ConnectFour cf;
    vector<string> board(6, string(7, '.'));
    int moves[] = {3, 0, 3, 1, 3, 0};                            // Y, R, Y, R, Y, R alternate
    for (int i = 0; i < 6; i++) {
        char p = i % 2 == 0 ? 'Y' : 'R';
        int r = cf.drop(board, moves[i], p);
        ok &= check(!cf.isWinningMove(board, r, moves[i], p), "no win before move 7");
    }
    int r = cf.drop(board, 3, 'Y');
    ok &= check(r == 2, "landing row stacks up");
    ok &= check(cf.isWinningMove(board, r, 3, 'Y'), "vertical win on move 7");
    vector<string> full(1, string(1, 'R'));
    ok &= check(cf.drop(full, 0, 'Y') == -1, "full column returns -1");
    return ok;
}

int main() {
    bool ok = true;
    ok &= test("horizontal", {".......",
                              ".......",
                              ".......",
                              ".......",
                              ".......",
                              "RRRR..."}, 5, 3, 'R', true);           // piece at the end of the run
    ok &= test("middle of run", {".......",
                                 ".......",
                                 ".......",
                                 ".......",
                                 ".......",
                                 ".RRRR.."}, 5, 2, 'R', true);        // run extends both ways from the new piece
    ok &= test("vertical", {".......",
                            ".......",
                            "...Y...",
                            "...Y...",
                            "...Y...",
                            "...Y..."}, 2, 3, 'Y', true);             // top piece of a column
    ok &= test("diagonal \\", {".......",
                               ".......",
                               "R......",
                               "YR.....",
                               "YYR....",
                               "YYYR..."}, 4, 2, 'R', true);          // down-right diagonal, new piece inside
    ok &= test("diagonal /", {".......",
                              ".......",
                              "...Y...",
                              "..YR...",
                              ".YRR...",
                              "YRRR..."}, 2, 3, 'Y', true);           // up-right diagonal
    ok &= test("three only", {".......",
                              ".......",
                              ".......",
                              ".......",
                              ".......",
                              "RRR.R.."}, 5, 2, 'R', false);          // gap breaks the run
    ok &= test("other colour", {".......",
                                ".......",
                                ".......",
                                ".......",
                                ".......",
                                "RRYR..."}, 5, 3, 'R', false);        // opponent piece in the run
    ok &= test("four elsewhere", {".......",
                                  ".......",
                                  ".......",
                                  ".......",
                                  "RRRR...",
                                  "Y.....R"}, 5, 6, 'R', false);      // a win elsewhere does not count for this move
    ok &= test("edge clip", {"RRRR"}, 0, 0, 'R', true);               // 1-row board, piece at the left edge
    ok &= test("tiny board", {"R"}, 0, 0, 'R', false);                // smallest board
    ok &= testDropGame();                                             // gravity follow-up
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
