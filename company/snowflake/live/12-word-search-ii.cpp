// B16 / LC 212: return every word from the list that can be traced on the board through 4-adjacent cells, no cell reused.

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
using std::unordered_set;

struct TrieNode {
    vector<int> next = vector<int>(26, -1);   // letter -> child index in pool, -1 = none
    string word;                              // word ending here; cleared once reported
};

class WordSearcher {
public:
    // trie of words + DFS from each cell walking board and trie together
    // T: O(S + m*n * 4*3^(L-1)), S: O(26*S) trie + O(L) recursion
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        buildTrie(words);
        vector<string> found;
        int m = board.size();
        int n = board[0].size();
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) { dfs(board, r, c, 0, found); }
        }
        return found;
    }

    // backtracking per distinct word (LC 79), T: O(W * m*n * 4*3^(L-1)), S: O(L) recursion + O(S) set
    vector<string> findWords2(vector<vector<char>>& board, vector<string>& words) {
        vector<string> found;
        unordered_set<string> done;   // words already tried
        int m = board.size();
        int n = board[0].size();
        for (const auto& w : words) {
            if (done.count(w)) { continue; }
            done.insert(w);
            bool hit = false;
            for (int r = 0; r < m && !hit; r++) {
                for (int c = 0; c < n && !hit; c++) { hit = exist(board, w, 0, r, c); }
            }
            if (hit) { found.push_back(w); }
        }
        return found;
    }

private:
    vector<TrieNode> pool;   // trie nodes; pool[0] is the root

    void buildTrie(const vector<string>& words) {
        pool.assign(1, TrieNode());
        for (const auto& w : words) {
            int cur = 0;
            for (char ch : w) {
                int k = ch - 'a';
                if (pool[cur].next[k] == -1) {
                    pool.push_back(TrieNode());   // may reallocate: hold indices, never TrieNode&
                    pool[cur].next[k] = pool.size() - 1;
                }
                cur = pool[cur].next[k];
            }
            pool[cur].word = w;
        }
    }

    // board[r][c] extends the prefix spelled by trie node `node`
    void dfs(vector<vector<char>>& board, int r, int c, int node, vector<string>& found) {
        char ch = board[r][c];
        if (ch == '#') { return; }                  // already on this path
        int child = pool[node].next[ch - 'a'];
        if (child == -1) { return; }                // no word has this prefix
        if (!pool[child].word.empty()) {
            found.push_back(pool[child].word);
            pool[child].word.clear();               // report once; keep going for longer words
        }
        board[r][c] = '#';
        int m = board.size();
        int n = board[0].size();
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            dfs(board, nr, nc, child, found);
        }
        board[r][c] = ch;                           // restore for other paths and starts
    }

    // does w[k..] start at (r, c)
    bool exist(vector<vector<char>>& board, const string& w, int k, int r, int c) {
        if (board[r][c] != w[k]) { return false; }  // '#' never equals a letter
        if (k + 1 == (int)w.size()) { return true; }
        char ch = board[r][c];
        board[r][c] = '#';
        int m = board.size();
        int n = board[0].size();
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        bool hit = false;
        for (int d = 0; d < 4 && !hit; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            hit = exist(board, w, k + 1, nr, nc);
        }
        board[r][c] = ch;                           // restore before returning, hit or not
        return hit;
    }
};

// expected is sorted; any output order is valid, so sort before comparing; board must come back unchanged
bool test(const string& label, vector<vector<char>> board, vector<string> words, const vector<string>& expected) {
    WordSearcher ws;
    vector<vector<char>> original = board;
    vector<string> got = ws.findWords(board, words);
    bool restored = board == original;
    vector<string> got2 = ws.findWords2(board, words);
    bool restored2 = board == original;
    std::sort(got.begin(), got.end());
    std::sort(got2.begin(), got2.end());
    if (got == expected && got2 == expected && restored && restored2) { return true; }
    cout << "FAIL " << label << ": sizes " << got.size() << " / " << got2.size()
         << ", expected " << expected.size() << ", board restored " << restored << " / " << restored2 << "\n";
    return false;
}

int main() {
    bool ok = true;
    ok &= test("lc1", {{'o', 'a', 'a', 'n'}, {'e', 't', 'a', 'e'}, {'i', 'h', 'k', 'r'}, {'i', 'f', 'l', 'v'}},
               {"oath", "pea", "eat", "rain"}, {"eat", "oath"});                        // LC example 1
    ok &= test("lc2", {{'a', 'b'}, {'c', 'd'}}, {"abcb"}, {});                           // LC example 2: needs reuse
    ok &= test("1x1", {{'a'}}, {"a", "b", "aa"}, {"a"});                                 // smallest board
    ok &= test("prefix chain", {{'a', 'b'}, {'c', 'd'}},
               {"ab", "ab", "abd", "abdc", "a", "acdb", "abc"},
               {"a", "ab", "abd", "abdc", "acdb"});                                       // dup word, prefixes, no diagonal
    ok &= test("many paths", {{'a', 'a'}, {'a', 'a'}}, {"aaaa", "aaaaa", "aa"}, {"aa", "aaaa"});   // dedupe, 5 > cells
    ok &= test("no reuse", {{'a', 'b', 'a'}}, {"aba", "abab", "bab"}, {"aba"});          // backtrack into used cell
    ok &= test("restore", {{'a', 'b'}}, {"ab", "ba"}, {"ab", "ba"});                      // later start needs restored cell
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
