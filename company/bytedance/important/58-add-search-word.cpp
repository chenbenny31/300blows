// ByteDance (hack2hire) / LC 211: WordDictionary with addWord(word) and search(word),
// where '.' in a search word matches any one letter.
#include <iostream>
#include <string>
#include <map>
using std::cout;
using std::string;
using std::map;

class WordDictionary {
    struct Node {
        bool isWord = false;        // a word ends at this node
        map<char, Node> children;   // next letter -> child node
    };
    Node root;   // empty prefix

    // match word[i..] from node; '.' tries every child.
    bool dfs(const Node& node, const string& word, int i) {
        if (i == (int)word.size()) { return node.isWord; }
        if (word[i] == '.') {
            for (const auto& [c, child] : node.children) {
                if (dfs(child, word, i + 1)) { return true; }
            }
            return false;
        }
        auto it = node.children.find(word[i]);
        if (it == node.children.end()) { return false; }
        return dfs(it->second, word, i + 1);
    }

public:
    // Walk the trie, creating missing nodes. T: O(L).
    void addWord(const string& word) {
        Node* cur = &root;
        for (char c : word) { cur = &cur->children[c]; }
        cur->isWord = true;
    }

    // DFS over the trie. T: O(L) without dots, O(trie nodes) worst case with dots.
    bool search(const string& word) { return dfs(root, word, 0); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    WordDictionary d;
    d.addWord("bad");
    d.addWord("dad");
    d.addWord("mad");
    check(!d.search("pad"), "missing");
    check(d.search("bad"), "exact");
    check(d.search(".ad"), "leading dot");
    check(d.search("b.."), "trailing dots");
}
