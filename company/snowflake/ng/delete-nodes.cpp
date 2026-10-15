#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::max;

struct Node {
    int val;
    vector<Node> children;
};

int dfs(const Node& cur, const unordered_set<int>& removed) {
    int maxDepth = 0;
    for (const Node& nxt : cur.children) {
        maxDepth = max(maxDepth, dfs(nxt, removed));
    }
    return maxDepth + (removed.count(cur.val) ? 0 : 1);
}

// T: O(n+D), S: O(D+h), D: deleted.size()
int height(const Node& root, const vector<int>& deleted) {
    unordered_set<int> removed(deleted.begin(), deleted.end());
    return dfs(root, removed);
}

void test(const string& label, const Node& root, const vector<int>& deleted, int want) {
    cout << label << (height(root, deleted) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1 -> {2, 3}, 3 -> {4}, 4 -> {5}
    Node root{1, {{2, {}}, {3, {{4, {{5, {}}}}}}}};
    test("no delete", root, {}, 4);
    test("promote", root, {3}, 3);
    test("all but root", root, {2, 3, 4, 5}, 1);
}
