// B16 · LC 212: return every word from `words` that can be traced on the board by
// moving up/down/left/right, never reusing a cell within one word.
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

struct TrieNode {
    map<char, TrieNode> children;   // next letter -> child node
    string word;                    // non-empty if a word ends here (cleared once found)
};

void dfs(vector<vector<char>>& board, int r, int c, TrieNode* cur, vector<string>& found) {
    int m = board.size(), n = board[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n) { return; }
    char ch = board[r][c];
    auto it = cur->children.find(ch);
    if (it == cur->children.end()) { return; }   // also stops on '#' (cell in use)
    TrieNode* nxt = &it->second;
    if (!nxt->word.empty()) {
        found.push_back(nxt->word);
        nxt->word.clear();                        // report each word once
    }
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    board[r][c] = '#';                            // mark in use for this path
    for (int d = 0; d < 4; d++) { dfs(board, r + dr[d], c + dc[d], nxt, found); }
    board[r][c] = ch;                             // restore on the way back
}

// Trie of words + DFS from every cell, pruned by trie prefixes.
// T: O(m*n * 4^L) worst case, L = longest word; S: O(total word length) for the trie.
vector<string> findWords(vector<vector<char>>& board, const vector<string>& words) {
    TrieNode root;
    for (const string& w : words) {
        TrieNode* cur = &root;
        for (char ch : w) { cur = &cur->children[ch]; }
        cur->word = w;
    }
    vector<string> found;
    for (int r = 0; r < (int)board.size(); r++) {
        for (int c = 0; c < (int)board[0].size(); c++) { dfs(board, r, c, &root, found); }
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
}
