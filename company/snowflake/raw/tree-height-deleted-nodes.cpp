#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <vector>

using std::vector;

struct Node {
    int val;
    vector<Node> children;
};

// T: O(n) expect, S: O(h) recur
int dfs(const Node& cur, const std::unordered_set<int>& removed) {
    int h = 0;
    for (const Node& child : cur.children) {
        h = std::max(h, dfs(child, removed));
    }
    return h + (removed.count(cur.val) ? 0 : 1);
}

// T: O(n+d) expect, S: O(d+h), d: del-size
int treeHeight(const Node& root, const vector<int>& deleted) {
    std::unordered_set<int> removed(deleted.begin(), deleted.end());
    return dfs(root, removed);
}

int main() {
    Node chain{1, {{2, {{3, {}}}}}};
    assert(treeHeight(chain, {2}) == 2); // promote 3 under 1
    // no del
    // only root survive
    // del leaf

    Node branch{1, {{2, {}}, {3, {}}}};
    assert(treeHeight(branch, {2}) == 2); // other branch remains

    // single node + no del
};
