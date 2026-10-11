// board of letters, list of words: return every word that can be traced through adjacent cells
// 4 directions, a cell is used at most once per word (LC212)

// lowercase a-z only?
// 4 directions? a cell reused within one word?
// duplicate words in the list? output order?
// may the board be modified during the search (restored after)?
// board size, word length bounds?

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::string;

class WordFinder {
private:
    struct TrieNode {
        vector<int> children = vector<int>(26, -1); // letter -> child index in pool, -1 = none
        int wordIdx = -1; // index into words if a word ends here, -1 = none or already reported
    };
    vector<TrieNode> pool; // pool[0] is the root

public:
    // trie of all words + one DFS per cell that walks the trie alongside the board
    // T: O(m*n * 4*3^(L-1)) worst, L: longest word; S: O(26 * total chars) trie + O(L) recursion
    // board taken by value: cells are marked during the search
    vector<string> findWords(vector<vector<char>> board, const vector<string>& words) {
        buildTrie(words);
        vector<string> found;
        if (board.empty()) {
            return found;
        }
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                dfs(board, r, c, 0, words, found);
            }
        }
        return found;
    }

private:
    void buildTrie(const vector<string>& words) {
        pool.assign(1, TrieNode());
        for (int wi = 0; wi < static_cast<int>(words.size()); wi++) {
            int cur = 0;
            for (char ch : words[wi]) {
                int k = ch - 'a';
                if (pool[cur].children[k] == -1) {
                    int child = static_cast<int>(pool.size());
                    pool.push_back(TrieNode()); // may reallocate pool
                    pool[cur].children[k] = child; // so index into pool only after the push
                }
                cur = pool[cur].children[k];
            }
            pool[cur].wordIdx = wi;
        }
    }

    // invariant: the path so far spells the prefix that leads to trie node `node`
    void dfs(vector<vector<char>>& board, int r, int c, int node,
             const vector<string>& words, vector<string>& found) {
        if (r < 0 || r >= static_cast<int>(board.size()) || c < 0 || c >= static_cast<int>(board[0].size())) {
            return;
        }
        char ch = board[r][c];
        if (ch == '#') {
            return; // already on this path
        }
        int child = pool[node].children[ch - 'a'];
        if (child == -1) {
            return; // no word continues with this letter: prune
        }
        if (pool[child].wordIdx != -1) {
            found.push_back(words[pool[child].wordIdx]);
            pool[child].wordIdx = -1; // report each word once
        }

        board[r][c] = '#';
        dfs(board, r + 1, c, child, words, found);
        dfs(board, r - 1, c, child, words, found);
        dfs(board, r, c + 1, child, words, found);
        dfs(board, r, c - 1, child, words, found);
        board[r][c] = ch; // restore for other paths
    }
};

int main() {
    WordFinder wf;
    vector<vector<char>> board = {{'a', 'b'},
                                  {'c', 'd'}};
    vector<string> want;

    want = {"abdc", "acdb"};
    vector<string> got = wf.findWords(board, {"abdc", "acdb", "abc"});
    std::sort(got.begin(), got.end()); // any order is valid
    assert(got == want); // target: two paths found, "abc" needs a diagonal

    want = {"ab"};
    assert(wf.findWords(board, {"ab", "ab", "aba"}) == want); // edge: duplicate word once, no cell reuse
    assert(wf.findWords(board, {"z"}).empty()); // fault: letter not on the board
    assert(wf.findWords({}, {"a"}).empty()); // null: empty board

    std::cout << "all tests passed\n";
    return 0;
}
