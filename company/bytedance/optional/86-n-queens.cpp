// ByteDance (hack2hire) / LC 51: return all ways to place n queens on an n x n board
// so that no two attack each other ('Q' = queen, '.' = empty).
// Follow-up LC 52: return only the number of solutions.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// places a queen in row r, then recurses on row r + 1
void place(int r, int n, vector<string>& board, vector<bool>& cols, vector<bool>& diag,
           vector<bool>& anti, vector<vector<string>>& result) {
    if (r == n) {
        result.push_back(board);
        return;
    }
    for (int c = 0; c < n; c++) {
        // r + c is constant on a '/' diagonal, r - c on a '\' one
        if (cols[c] || diag[r + c] || anti[r - c + n - 1]) { continue; }
        cols[c] = diag[r + c] = anti[r - c + n - 1] = true;
        board[r][c] = 'Q';
        place(r + 1, n, board, cols, diag, anti, result);
        board[r][c] = '.';
        cols[c] = diag[r + c] = anti[r - c + n - 1] = false;
    }
}

// Row-by-row backtracking with column / diagonal flags. T: O(n!), S: O(n^2).
vector<vector<string>> solveNQueens(int n) {
    vector<string> board(n, string(n, '.'));   // current partial placement
    vector<bool> cols(n, false);               // cols[c] = column c taken
    vector<bool> diag(2 * n - 1, false);       // diag[r + c] = '/' diagonal taken
    vector<bool> anti(2 * n - 1, false);       // anti[r - c + n - 1] = '\' diagonal taken
    vector<vector<string>> result;
    place(0, n, board, cols, diag, anti, result);
    return result;
}

int countFrom(int r, int n, vector<bool>& cols, vector<bool>& diag, vector<bool>& anti) {
    if (r == n) { return 1; }
    int total = 0;
    for (int c = 0; c < n; c++) {
        if (cols[c] || diag[r + c] || anti[r - c + n - 1]) { continue; }
        cols[c] = diag[r + c] = anti[r - c + n - 1] = true;
        total += countFrom(r + 1, n, cols, diag, anti);
        cols[c] = diag[r + c] = anti[r - c + n - 1] = false;
    }
    return total;
}

// follow-up: same backtracking without a board. T: O(n!), S: O(n).
int totalNQueens(int n) {
    vector<bool> cols(n, false);           // cols[c] = column c taken
    vector<bool> diag(2 * n - 1, false);   // diag[r + c] = '/' diagonal taken
    vector<bool> anti(2 * n - 1, false);   // anti[r - c + n - 1] = '\' diagonal taken
    return countFrom(0, n, cols, diag, anti);
}

void test(const string& label, int n, const vector<vector<string>>& want) {
    cout << label << (solveNQueens(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("n = 4", 4, {{".Q..", "...Q", "Q...", "..Q."},
                      {"..Q.", "Q...", "...Q", ".Q.."}});
    test("n = 1", 1, {{"Q"}});
}
