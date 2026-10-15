// B16 · LC 212 (variant): same trie + DFS, but the prefix check happens BEFORE
// recursing, so the node passed into dfs already includes the current cell.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::sort;

struct Node {
    map<char, Node> children;   // next letter -> child node
    string word;                    // non-empty if a word ends here (cleared once found)
};

// cur already includes board[r][c]: step into the trie BEFORE recursing.
void dfs(vector<vector<char>>& board, int r, int c, Node* cur,
         vector<string>& found) {
    if (!cur->word.empty()) {
        found.push_back(cur->word);
        cur->word.clear();                        // report each word once
    }
    int m = board.size(), n = board[0].size();
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char ch = board[r][c];
    board[r][c] = '#';                            // mark in use for this path
    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
        auto it = cur->children.find(board[nr][nc]);
        if (it == cur->children.end()) { continue; }   // also skips '#' cells
        dfs(board, nr, nc, &it->second, found);
    }
    board[r][c] = ch;                             // restore on the way back
}

vector<string> findWords(vector<vector<char>>& board, const vector<string>& words) {
    Node root;
    for (const string& w : words) {
        Node* cur = &root;
        for (char ch : w) { cur = &cur->children[ch]; }
        cur->word = w;
    }
    vector<string> found;
    for (int r = 0; r < (int)board.size(); r++) {
        for (int c = 0; c < (int)board[0].size(); c++) {
            // step into the trie for the starting cell before the first call
            auto it = root.children.find(board[r][c]);
            if (it != root.children.end()) { dfs(board, r, c, &it->second, found); }
        }
    }
    return found;
}

void test(const string& label, vector<vector<char>> board, const vector<string>& words,
          vector<string> want) {
    vector<string> got = findWords(board, words);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("shared prefix", {{'c', 'a', 't'},
                           {'x', 'r', 't'}}, {"cat", "car", "cart", "dog"},
         {"car", "cart", "cat"});
    test("no reuse", {{'a', 'b'}}, {"aba"}, {});
    test("one letter", {{'a'}}, {"a"}, {"a"});
}
