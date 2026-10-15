// B01 · LC 559 height prototype: N-ary tree, unique values; delete listed nodes (children move up, root never deleted); return height in nodes.
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::vector;
using std::unordered_set;

struct Node {
    int val;
    vector<Node> children;
};

// postorder; a deleted node contributes 0 to its subtree height, T: O(V), S: O(H)
int dfs(const Node& cur, const unordered_set<int>& removed) {
    int tallest = 0;                      // tallest child subtree
    for (const Node& child : cur.children) { tallest = std::max(tallest, dfs(child, removed)); }
    return tallest + (removed.count(cur.val) ? 0 : 1);
}

// promotion keeps every root-to-leaf path, a deleted node just stops counting, T: O(V+D), S: O(D+H)
int height(const Node& root, const vector<int>& deleted) {
    unordered_set<int> removed(deleted.begin(), deleted.end());   // values to delete
    return dfs(root, removed);
}

void test(const Node& root, const vector<int>& deleted, int expect) {
    int got = height(root, deleted);
    cout << (got == expect ? "PASS " : "FAIL ") << got << " expect " << expect << "\n";
}

int main() {
    Node root{1, {{2, {{6, {}}}}, {3, {{4, {{5, {}}}}}}}};   // 1 -> {2, 3}, 2 -> {6}, 3 -> {4}, 4 -> {5}
    test(root, {}, 4);                                      // plain height 1-3-4-5
    test(root, {3}, 3);                                     // 4 promoted, not subtree cut: 1-4-5
    test(root, {3, 4}, 3);                                  // tallest path switches to 1-2-6
    test(root, {2, 3, 4, 5, 6}, 1);                         // root alone
    test(Node{7, {}}, {99}, 1);                             // single node, unknown value ignored
}
