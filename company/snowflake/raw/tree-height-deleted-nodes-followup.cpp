#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <utility>
#include <vector>

using std::vector;
using std::pair;

struct Node {
    int val;
    vector<Node> children;
};

// T: O(n+d) expect, S: O(n+d), d: del-size
int treeHeight(const Node& root, const vector<int>& deleted) {
    std::unordered_set<int> removed(deleted.begin(), deleted.end());
    vector<pair<const Node*, int>> stk;

    stk.push_back({&root, 1}); // root cannot be del
    int maxH = 0;

    while (!stk.empty()) {
        auto [cur, h] = stk.back();
        stk.pop_back();
        maxH = std::max(maxH, h);
        for (const auto& child : cur->children) {
            int nxtH = h + (removed.count(child.val) ? 0 : 1);
            stk.push_back({&child, nxtH});
        }
    }
    return maxH;
}

int main() {
    Node chain{1, {{2, {{3, {{4, {}}}}}}}};
    assert(treeHeight(chain, {2, 3}) == 2);
    // no del, full chain
    // only root survive

    Node branch{1, {{2, {}}, {3, {}}}};
    assert(treeHeight(branch, {2}) == 2);

    // single node tree, no del
}
