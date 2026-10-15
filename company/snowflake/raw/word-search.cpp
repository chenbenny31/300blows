
#include <algorithm>
#include <cassert>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class WordFinder {
    struct Node {
        map<char, Node> children; // Letter -> next prefix.
        int wordIndex = -1;       // Completed word; -1 means none/reported.
    };

    void dfs(vector<vector<char>>& board, int r, int c, Node& node,
             const vector<string>& words, vector<string>& result) {
        if (r < 0 || r >= static_cast<int>(board.size()) ||
            c < 0 || c >= static_cast<int>(board[0].size())) {
            return;
        }

        char letter = board[r][c];
        if (letter == '#') {
            return;
        }

        auto it = node.children.find(letter);
        if (it == node.children.end()) {
            return;
        }
        Node& next = it->second;

        if (next.wordIndex != -1) {
            result.push_back(words[next.wordIndex]);
            next.wordIndex = -1; // Report this word only once.
        }

        board[r][c] = '#';
        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; ++d) {
            dfs(board, r + dr[d], c + dc[d], next, words, result);
        }
        board[r][c] = letter;
    }

public:
    // T: O(S + R*C*4^L), S: O(S + L), excluding output.
    // S = total dictionary characters, L = longest word.
    // R, C = board dimensions.
    vector<string> findWords(vector<vector<char>>& board,
                             const vector<string>& words) {
        Node root;
        for (int i = 0; i < static_cast<int>(words.size()); ++i) {
            Node* cur = &root;
            for (char letter : words[i]) {
                cur = &cur->children[letter];
            }
            cur->wordIndex = i;
        }

        vector<string> result;
        if (board.empty() || board[0].empty()) {
            return result;
        }

        for (int r = 0; r < static_cast<int>(board.size()); ++r) {
            for (int c = 0; c < static_cast<int>(board[0].size()); ++c) {
                dfs(board, r, c, root, words, result);
            }
        }
        return result;
    }
};

int main() {
    WordFinder finder;
    vector<vector<char>> board = {{'a', 'b'}, {'c', 'd'}};

    auto result = finder.findWords(
        board, {"ab", "abdc", "acdb", "abc", "aba", "ab"});
    std::sort(result.begin(), result.end());

    vector<string> expected = {"ab", "abdc", "acdb"};
    assert(result == expected);
    // Shared prefix, duplicate word, no diagonals, no cell reuse.

    vector<vector<char>> original = {{'a', 'b'}, {'c', 'd'}};
    assert(board == original); // Backtracking restores the board.

    assert(finder.findWords(board, {"z"}).empty()); // Repeated call.
    vector<vector<char>> empty;
    assert(finder.findWords(empty, {"a"}).empty());
}
