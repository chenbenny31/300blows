// multi-way tree + delete list: a deleted node adds 0 to a path, its children move up to its parent
// return the height counted in nodes, root is never deleted, values are unique

#include <vector>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <cassert>
#include <iostream>

using std::vector;
using std::unordered_set;

struct Node {
    int val;
    vector<Node> children;
};

class TreeHeightCalculator {
public:
    // bottom-up DFS, delete node adds 0, T: O(V+D), S: O(D+H), D: size of delete list, H: tree height
    int treeHeight_1(const Node& root, const vector<int>& deleted) {
        unordered_set<int> removed(deleted.begin(), deleted.end()); // ids to delete, O(1) lookup
        return dfs(root, removed);
    }

private:
    int dfs(const Node& cur, const unordered_set<int>& removed) {
        int maxH = 0; // tallest child path
        for (const Node& child : cur.children) {
            maxH = std::max(maxH, dfs(child, removed));
        }
        return maxH + (removed.count(cur.val) ? 0 : 1); // deleted adds 0
    }

public:
    // top-down explicit stack, T: (V+D), S: O(V+D)
    int treeHeight_2(const Node& root, const vector<int>& deleted) {
        unordered_set<int> removed(deleted.begin(), deleted.end());
        int maxH = 0;
        vector<std::pair<const Node*, int>> stk; // {node, height of root..node}
        stk.push_back({&root, 1}); // root is never deleted
        while (!stk.empty()) {
            auto [cur, h] = stk.back();
            stk.pop_back();
            maxH = std::max(maxH, h);
            for (const Node& child : cur->children) {
                stk.push_back({&child, h + (removed.count(child.val) ? 0 : 1)});
            }
        }
        return maxH;
    }
};

int main() {
    TreeHeightCalculator th;

    // 1 -> {2, 3}, 3 -> 4, 4 -> 5, longest path 1-3-4-5
    Node root{1, {{2, {}}, {3, {{4, {{5, {}}}}}}}};

    assert(th.treeHeight_1(root, {}) == 4); // no del
    assert(th.treeHeight_2(root, {}) == 4);

    assert(th.treeHeight_1(root, {3}) == 3); // del 3, make 4 promoted under 1, path 1-4-5
    assert(th.treeHeight_2(root, {3}) == 3);

    assert(th.treeHeight_1(root, {3,4}) == 2); // del 3,4, promote 5 under 1, path 1-5
    assert(th.treeHeight_2(root, {3,4}) == 2);

    assert(th.treeHeight_1(root, {2,3,4,5}) == 1); // del all except root
    assert(th.treeHeight_2(root, {2,3,4,5}) == 1);

    Node single{1, {}};
    assert(th.treeHeight_1(single, {}) == 1); // single root
    assert(th.treeHeight_2(single, {}) == 1); // single root

    std::cout << "all tests passed\n";
    return 0;
}
