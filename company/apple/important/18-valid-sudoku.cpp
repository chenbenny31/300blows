// LC 36: is a partially filled 9x9 board valid? Each row, column and 3x3 box must hold
// digits 1-9 without repeats; '.' is empty. Solvability is not required.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// One pass with seen flags per row, column and box. T: O(81), S: O(81).
bool isValidSudoku(const vector<vector<char>>& board) {
    // rows[r][d] = digit d+1 already in row r; same for cols and boxes
    vector<vector<bool>> rows(9, vector<bool>(9, false));
    vector<vector<bool>> cols(9, vector<bool>(9, false));
    vector<vector<bool>> boxes(9, vector<bool>(9, false));
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (board[r][c] == '.') { continue; }
            int d = board[r][c] - '1';
            int b = (r / 3) * 3 + c / 3;   // box index 0..8, row-major
            if (rows[r][d] || cols[c][d] || boxes[b][d]) { return false; }
            rows[r][d] = cols[c][d] = boxes[b][d] = true;
        }
    }
    return true;
}

void test(const string& label, const vector<string>& lines, bool want) {
    vector<vector<char>> board;
    for (const auto& line : lines) { board.push_back({line.begin(), line.end()}); }
    cout << label << (isValidSudoku(board) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<string> valid = {"53..7....", "6..195...", ".98....6.",
                            "8...6...3", "4..8.3..1", "7...2...6",
                            ".6....28.", "...419..5", "....8..79"};
    test("lc ex1", valid, true);
    vector<string> boxDup = valid;
    boxDup[1][1] = '8';
    test("box dup", boxDup, false);
}
