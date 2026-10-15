// B01: N-ary tree with unique values; delete every node in a list (root never deleted), each deleted node's
// children move up to its parent. Return the resulting height counted in nodes.

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
using std::pair;
using std::unordered_set;

struct Node {
    int val;
    vector<Node> children;
};

class TreeHeightCalculator {
public:
    // postorder: promotion keeps every root-to-leaf path, a deleted node just stops counting, T: O(V+D), S: O(D+H)
    int height(const Node& root, const vector<int>& deleted) {
        unordered_set<int> removed(deleted.begin(), deleted.end());   // values to delete
        return dfs(root, removed);
    }

    // top-down explicit stack carrying the depth so far, for chain-shaped trees, T: O(V+D), S: O(V+D)
    int height2(const Node& root, const vector<int>& deleted) {
        unordered_set<int> removed(deleted.begin(), deleted.end());   // values to delete
        int best = 0;
        vector<pair<const Node*, int>> stk;                           // {node, surviving nodes on root..node}
        stk.push_back({&root, 1});                                    // root is never deleted
        while (!stk.empty()) {
            auto [cur, h] = stk.back();                               // copy before pop_back
            stk.pop_back();
            best = std::max(best, h);
            for (const Node& child : cur->children) {
                int add = removed.count(child.val) ? 0 : 1;
                stk.push_back({&child, h + add});
            }
        }
        return best;
    }

private:
    // height of the subtree after deletions; a deleted node contributes 0
    int dfs(const Node& cur, const unordered_set<int>& removed) {
        int tallest = 0;                                              // tallest child subtree
        for (const Node& child : cur.children) { tallest = std::max(tallest, dfs(child, removed)); }
        return tallest + (removed.count(cur.val) ? 0 : 1);
    }
};

bool test(const string& label, const Node& root, const vector<int>& deleted, int expected) {
    TreeHeightCalculator th;
    int got = th.height(root, deleted);
    int got2 = th.height2(root, deleted);
    if (got == expected && got2 == expected) { return true; }
    cout << "FAIL " << label << ": got " << got << " / " << got2 << ", expected " << expected << "\n";
    return false;
}

int main() {
    bool ok = true;
    // 1 -> {2, 3}, 2 -> {6}, 3 -> {4}, 4 -> {5}; longest path 1-3-4-5
    Node root{1, {{2, {{6, {}}}}, {3, {{4, {{5, {}}}}}}}};
    ok &= test("no deletes", root, {}, 4);                    // plain height
    ok &= test("delete 3", root, {3}, 3);                     // 4 promoted under 1: 1-4-5
    ok &= test("delete 3, 4", root, {3, 4}, 3);               // tallest path switches to 1-2-6
    ok &= test("delete 3, 4, 2", root, {3, 4, 2}, 2);         // 5 and 6 both hang off the root
    ok &= test("delete leaf", root, {5}, 3);                  // leaf removed, not a subtree cut
    ok &= test("delete all but root", root, {2, 3, 4, 5, 6}, 1);   // root alone
    ok &= test("unknown value", root, {99}, 4);               // value not in tree is ignored
    Node single{7, {}};
    ok &= test("single", single, {}, 1);                      // smallest input
    // wide: deleting a middle node whose children have different heights
    Node wide{1, {{2, {{3, {}}, {4, {{5, {}}}}}}}};
    ok &= test("promote all children", wide, {2}, 3);         // 4 and 3 both move up; 1-4-5
    cout << (ok ? "all tests passed\n" : "some tests FAILED\n");
    return ok ? 0 : 1;
}
